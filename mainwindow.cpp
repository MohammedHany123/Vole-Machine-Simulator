#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QRegularExpression>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QEventLoop>
#include <QTimer>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->HelpMenu->setVisible(false);
    ui->FetchMemory->setAutoRepeat(false);
    ui->Write->setVisible(false);
    ui->LoadFile->setVisible(false);
    connect(ui->LoadFile, &QPushButton::clicked, this, &MainWindow::on_LoadFile_clicked);
    ui->InstructionInsert->setVisible(false);
    ui->CancelInstruction->setVisible(false);
    ui->InstructionShow->setVisible(false);
    ui->OKInstruction->setVisible(false);
    connect(ui->OKInstruction, &QPushButton::clicked, this, &MainWindow::on_OKInstruction_clicked);
    ui->RString->setVisible(false);
    ui->RString_2->setVisible(false);
    ui->xString->setVisible(false);
    ui->XString->setVisible(false);
    ui->SString->setVisible(false);
    ui->xString_2->setVisible(false);
    ui->XString_2->setVisible(false);
    ui->YString->setVisible(false);
    ui->TString->setVisible(false);
    ui->SString_2->setVisible(false);
    ui->xString_3->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_HelpButton_clicked()
{
    ui->HelpMenu->setVisible(true);
}


void MainWindow::on_CancelMenu_clicked()
{
    ui->HelpMenu->setVisible(false);
}

Registers::Registers() : size(16){
    for(int i = 0; i < size; i++){
        Registers::reg[i] = "00";
    }
}

void Registers::setRegister(int address, const std::string &val)
{
    if (address >= 0 && address < size) Registers::reg[address] = val;
}

std::string Registers::getRegister(int address)
{
    if (address >= 0 && address < size) return Registers::reg[address];
    return "";
}

void Registers::clearRegister(){
    for (int i = 0; i < size; i++)
    {
        Registers::reg[i] = "00";
    }
}

Memory::Memory() : size(256){
    for(int i = 0; i < size; i++){
        Memory::mem[i] = "00";
    }
}

std::string Memory::getMemory(int address)
{
    if (address >= 0 && address < size) return Memory::mem[address];
    return "";
}

void Memory::setMemory(int address, const std::string &val)
{
    if (address >= 0 && address < size) Memory::mem[address] = val;
}

void Memory::clearMemory(){
    for (int i = 0; i < size; i++)
    {
        Memory::mem[i] = "00";
    }
}

void CPU::setPC(std::string val)
{
    pc = val;
}

std::string CPU::fetch(Memory &memory){
    int address = std::stoi(pc, nullptr, 16);
    std::string byte1 = memory.getMemory(address);
    QString b1 = QString::fromStdString(byte1).left(2).toUpper();
    std::string bt1 = b1.toStdString();
    std::string byte2 = memory.getMemory(address+1);
    QString b2 = QString::fromStdString(byte2).left(2).toUpper();
    std::string bt2 = b2.toStdString();
    std::string InstructionHex = bt1+bt2;
    ir = InstructionHex;
    address += 2;
    pc = QString::number(address, 16).toUpper().rightJustified(2, '0').toStdString();
    return ir;
}

std::string CPU::getPC()
{
    return pc;
}

std::string CPU::getIR()
{
    return ir;
}


int CPU::hex_string_to_int(const QString &hexStr)
{
    return hexStr.toInt(nullptr, 16);
}

QString CPU::int_to_hex_string(int value)
{
    return QString("%1").arg(value & 0xFF, 2, 16, QChar('0')).toUpper();
}

void Instruction::setInstruct(const std::string &code)
{
    instruct = code;
}

void Instruction::clearOutput()
{
    outputLine = "";
}

void Instruction::load(CPU &cpu, Memory &memory, Registers &registers, const std::string &R, const std::string &XY)
{
    int reg_index = cpu.hex_string_to_int(QString::fromStdString(R));
    int ram_address = cpu.hex_string_to_int(QString::fromStdString(XY));
    registers.setRegister(reg_index, memory.getMemory(ram_address));
}

void Instruction::copy(CPU &cpu, Memory &memory, Registers &registers, const std::string &R, const std::string &XY)
{
    int reg_index = cpu.hex_string_to_int(QString::fromStdString(R));
    registers.setRegister(reg_index, XY);
}

void Instruction::store(CPU &cpu, Memory &memory, Registers &registers, const std::string &R, const std::string &XY)
{
    int reg_index=cpu.hex_string_to_int(QString::fromStdString(R));
    int ram_address=cpu.hex_string_to_int(QString::fromStdString(XY));
    memory.setMemory(ram_address, registers.getRegister(reg_index));
}

void Instruction::move(CPU &cpu, Registers &registers, const std::string &R, const std::string &S)
{
    int R_index = cpu.hex_string_to_int(QString::fromStdString(R));
    int S_index = cpu.hex_string_to_int(QString::fromStdString(S));
    registers.setRegister(S_index, registers.getRegister(R_index));
}

void Instruction::jump(CPU &cpu, Registers &registers, const std::string &R, const std::string &XY)
{
    int reg_index = cpu.hex_string_to_int(QString::fromStdString(R));
    if (registers.getRegister(reg_index) == registers.getRegister(0)) {
        cpu.setPC(XY);
    }
}

void Instruction::jumpg(CPU &cpu, Registers &registers, const std::string &R, const std::string &XY)
{
    int reg_index = cpu.hex_string_to_int(QString::fromStdString(R));
    if(registers.getRegister(reg_index) > registers.getRegister(0)){
        cpu.setPC(XY);
    }
}

void Instruction::halt()
{
    haltFlag = !haltFlag;
}

bool Instruction::isHalted() const
{
    return haltFlag;
}

void Instruction::rotateRight(CPU &cpu, Registers &registers, const std::string &R, const std::string &X)
{
    int srcAddress = cpu.hex_string_to_int(QString::fromStdString(R));
    std::string srcValue = registers.getRegister(srcAddress);
    uint8_t byte = cpu.hex_string_to_int(QString::fromStdString(srcValue));
    int xInt = QString::fromStdString(X).toInt();
    int xxInt = 8-xInt;
    uint8_t rotated = (byte >> xInt) | (byte << xxInt);
    int destAddress = cpu.hex_string_to_int(QString::fromStdString(R));
    registers.setRegister(destAddress, cpu.int_to_hex_string(rotated).toStdString());
}

void Instruction::andOperation(CPU &cpu, Registers &registers, const std::string &R, const std::string &S, const std::string& T)
{
    uint8_t val1 = cpu.hex_string_to_int(QString::fromStdString(registers.getRegister(QString::fromStdString(S).toInt())));
    uint8_t val2 = cpu.hex_string_to_int(QString::fromStdString(registers.getRegister(QString::fromStdString(T).toInt())));

    uint8_t result = val1 & val2;

    registers.setRegister(cpu.hex_string_to_int(QString::fromStdString(R)), cpu.int_to_hex_string(result).toStdString());
}

void Instruction::orOperation(CPU &cpu, Registers &registers, const std::string &R, const std::string &S, const std::string& T)
{
    uint8_t val1 = cpu.hex_string_to_int(QString::fromStdString(registers.getRegister(QString::fromStdString(S).toInt())));
    uint8_t val2 = cpu.hex_string_to_int(QString::fromStdString(registers.getRegister(QString::fromStdString(T).toInt())));

    uint8_t result = val1 | val2;

    registers.setRegister(cpu.hex_string_to_int(QString::fromStdString(R)), cpu.int_to_hex_string(result).toStdString());
}

void Instruction::xorRotation(CPU &cpu, Registers &registers, const std::string &R, const std::string &S, const std::string &T)
{
    uint8_t val1 = cpu.hex_string_to_int(QString::fromStdString(registers.getRegister(QString::fromStdString(S).toInt())));
    uint8_t val2 = cpu.hex_string_to_int(QString::fromStdString(registers.getRegister(QString::fromStdString(T).toInt())));

    uint8_t result = val1 ^ val2;

    registers.setRegister(cpu.hex_string_to_int(QString::fromStdString(R)), cpu.int_to_hex_string(result).toStdString());

}

void Instruction::addOperationTwo(CPU &cpu, Registers &registers, const std::string &R, const std::string &S, const std::string &T)
{
    uint8_t val1 = cpu.hex_string_to_int(QString::fromStdString(registers.getRegister(cpu.hex_string_to_int(QString::fromStdString(S)))));
    uint8_t val2 = cpu.hex_string_to_int(QString::fromStdString(registers.getRegister(cpu.hex_string_to_int(QString::fromStdString(T)))));

    uint8_t result = val1 + val2;

    registers.setRegister(cpu.hex_string_to_int(QString::fromStdString(R)), cpu.int_to_hex_string(result & 0xFF).toStdString());
}

void Instruction::addOperationFloating(CPU &cpu, Registers &registers, const std::string &R, const std::string &S, const std::string &T)
{
    uint8_t val1Hex = cpu.hex_string_to_int(QString::fromStdString(registers.getRegister(cpu.hex_string_to_int(QString::fromStdString(S)))));
    uint8_t val2Hex = cpu.hex_string_to_int(QString::fromStdString(registers.getRegister(cpu.hex_string_to_int(QString::fromStdString(T)))));

    union {
        uint8_t i;
        float f;
    } val1, val2, res;

    val1.i = val1Hex;
    val2.i = val2Hex;

    res.f = static_cast<float>(val1.i) + static_cast<float>(val2.i);

    res.i = static_cast<uint8_t>(res.f);

    QString result = QString::number(res.i, 16).toUpper().rightJustified(2, '0');

    registers.setRegister(cpu.hex_string_to_int(QString::fromStdString(R)), result.toStdString());
}

QString Instruction::execute(CPU& cpu, Memory& memory, Registers& registers)
{
    char opcode = instruct[0];
    std::string R(1, instruct[1]);
    std::string S(1, instruct[2]);
    std::string T(1, instruct[3]);
    std::string XY = S + T;

    switch (opcode) {
    case '1':  // Load value from RAM[XY] to register R
        load(cpu, memory, registers, R, XY);
        break;
    case '2':  // Copy bit-string XY to register R
        copy(cpu, memory, registers, R, XY);
        break;
    case '3':  // Store value from register R to RAM[XY]
        if (XY == "00")
        {
            int regIndex = cpu.hex_string_to_int(QString::fromStdString(R));
            QString value = QString::fromStdString(registers.getRegister(regIndex));

           if (!value.isEmpty()) {
                bool ok;
                int intValue = value.toInt(&ok, 16); // Convert hex string to integer (base 16)
                if (ok) {
                    char asciiChar = static_cast<char>(intValue); // Convert integer to ASCII character
                    outputLine = outputLine + QString(asciiChar); // Store as QString for output
                }
            }
            store(cpu, memory, registers, R, XY);
        }
        else
        {
            store(cpu, memory, registers, R, XY);
        }
        break;
    case '4':  // Move value from register R to register S
        move(cpu, registers, S, T);
        break;
    case '5':
        addOperationTwo(cpu, registers, R, S, T);
        break;
    case '6':
        addOperationFloating(cpu, registers, R, S, T);
        break;
    case '7':
        orOperation(cpu, registers, R, S, T);
        break;
    case '8':
        andOperation(cpu, registers, R, S, T);
        break;
    case '9':
        xorRotation(cpu, registers, R, S, T);
        break;
    case 'A':
        rotateRight(cpu, registers, R, T);
        break;
    case 'B':  // Jump to address XY if register R is zero
        jump(cpu, registers, R, XY);
        break;
    case 'C':  // Halt CPU execution
        halt();
        break;
    case 'D':  // Jump to address XY if register R is zero
        jumpg(cpu, registers, R, XY);
        break;
    default:
        break;
    }
    return outputLine;
}

void MainWindow::on_FetchMemory_clicked()
{
    for (int i = 0; i < 256; ++i) {
        QString memoryAddress = QString("MEM%1").arg(i, 2, 16, QChar('0')).toUpper();

        QTextEdit* memoryTextEdit = findChild<QTextEdit*>(memoryAddress); // This assumes each memory location has a corresponding QTextEdit named MEM00, MEM01, etc.

        if (memoryTextEdit) {
            QString textValue = memoryTextEdit->toPlainText().trimmed(); // Get the text from the QTextEdit
            memory.setMemory(i, textValue.toStdString()); // Store the value in memory at index i
        }
    }
    bool ok;
    int address = ui->PC->toPlainText().trimmed().toInt(&ok, 16);

    if (!ok || address == 0) {
        QMessageBox::warning(this, "Input Error", "PC value is not a valid hexadecimal number.");
        return;
    }
    std::string pc = ui->PC->toPlainText().trimmed().toStdString();
    cpu.setPC(pc);
    ui->IR->setText(QString::fromStdString(cpu.fetch(memory)));
    ui->PC->setText(QString::fromStdString(cpu.getPC()));
    ui->PC->setAlignment(Qt::AlignCenter);
    ui->IR->setAlignment(Qt::AlignCenter);
}


void MainWindow::on_ClearRegister_clicked()
{
    registers.clearRegister();
    showRegisters();
}

void MainWindow::on_ClearMemory_clicked()
{
    ui->RString->setVisible(false);
    ui->RString_2->setVisible(false);
    ui->xString->setVisible(false);
    ui->XString->setVisible(false);
    ui->SString->setVisible(false);
    ui->xString_2->setVisible(false);
    ui->XString_2->setVisible(false);
    ui->YString->setVisible(false);
    ui->TString->setVisible(false);
    ui->SString_2->setVisible(false);
    ui->xString_3->setVisible(false);
    ui->CUScreen->clear();
    ui->ALUScreen->clear();
    memory.clearMemory();
    showMemory();
    ui->Screen->clear();
    ui->R->clear();
    ui->X->clear();
    ui->Opcode->clear();
    ui->Y->clear();
    ui->IR->clear();
    ir.clearOutput();
}

void MainWindow::showRegisters()
{
    for (int i = 0; i < 16; ++i) {
        QString labelName = QString("Reg%1").arg(i, 0, 16).toUpper();
        QTextEdit *label = findChild<QTextEdit*>(labelName);
        if (label) {
            QString value = QString::fromStdString(registers.getRegister(i));
            value = value.left(2).rightJustified(2, '0');
            label->setText(value);

            QTextCursor cursor(label->textCursor());
            QTextBlockFormat blockFormat;
            blockFormat.setAlignment(Qt::AlignCenter);
            cursor.select(QTextCursor::Document);
            cursor.mergeBlockFormat(blockFormat);
        }
    }
}

void MainWindow::showMemory()
{
    for (int i = 0; i < 256; ++i) {
        QString labelName = QString("MEM%1").arg(i, 2, 16, QChar('0')).toUpper();  // Generate widget name like "MEM00", "MEM01", ..., "MEMFF"
        QTextEdit *label = findChild<QTextEdit*>(labelName);  // Find the QTextEdit by name
        if (label) {
            QString value = QString::fromStdString(memory.getMemory(i));
            value = value.left(2).rightJustified(2, '0');
            label->setText(value);  // Set text if QTextEdit is found

            QTextCursor cursor(label->textCursor());
            QTextBlockFormat blockFormat;
            blockFormat.setAlignment(Qt::AlignCenter);  // Set alignment to center
            cursor.select(QTextCursor::Document);
            cursor.mergeBlockFormat(blockFormat);
        }
    }

}

void MainWindow::on_LoadCode_clicked()
{
    ui->LoadCode->setVisible(false);
    ui->LoadFile->setVisible(true);
    ui->Write->setVisible(true);
}


void MainWindow::on_LoadFile_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open Instruction File", "", "Text Files (*.txt)");
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "File Error", "Cannot open file for reading");
            return;
        }

        QTextStream in(&file);
        bool ok;
        int address = ui->PC->toPlainText().trimmed().toInt(&ok, 16);

        if (!ok || address == 0) {
            QMessageBox::warning(this, "Input Error", "PC value is not a valid hexadecimal number.");
            return;
        }
        QString line = in.readLine().trimmed();
        QStringList instructions = line.split(" ");

        QRegularExpression hexRegex("^[0-9A-DFa-df]{4}$");
        for (const QString &word : instructions) {
            if (word.length() != 4 || !hexRegex.match(word).hasMatch() || address + 1 >= 256) {
                QMessageBox::critical(this, "Error", "An invalid instruction is inserted.");
                return;
            }

            QString byte1 = word.mid(0, 2);
            QString byte2 = word.mid(2, 2);

            memory.setMemory(address, byte1.toStdString());
            memory.setMemory(address + 1, byte2.toStdString());

            address += 2;
        }

        file.close();
        showMemory();
    }
    ui->LoadFile->setVisible(false);
    ui->Write->setVisible(false);
    ui->LoadCode->setVisible(true);
}


void MainWindow::on_Write_clicked()
{

    ui->InstructionInsert->setVisible(true);
    ui->InstructionShow->setVisible(true);
    ui->CancelInstruction->setVisible(true);
    ui->OKInstruction->setVisible(true);
}

void MainWindow::on_OKInstruction_clicked()
{
    QRegularExpression hexRegex("^[0-9A-DFa-df]{4}$");
    QString instructionLine = ui->InstructionInsert->toPlainText().trimmed();
    if (instructionLine.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter an instruction.");
        return;
    }
    bool ok;
    int address = ui->PC->toPlainText().trimmed().toInt(&ok, 16);

    if (!ok || address == 0) {
        QMessageBox::warning(this, "Input Error", "PC value is not a valid hexadecimal number.");
        return;
    }
    QStringList instructions = instructionLine.split(" ");
    for (const QString &word : instructions) {
        if (word.length() != 4 || !hexRegex.match(word).hasMatch() || address + 1 >= 256) {
            QMessageBox::warning(this, "Input Error", "Each instruction must be 4 characters in valdi hexadecimal format (e.g., 2000).");
            return;
        }
        QString byte1 = word.mid(0, 2);
        QString byte2 = word.mid(2, 2);

        memory.setMemory(address, byte1.toStdString());
        memory.setMemory(address + 1, byte2.toStdString());

        address += 2;
    }
    ui->InstructionInsert->clear();
    showMemory();
    ui->InstructionInsert->setVisible(false);
    ui->InstructionShow->setVisible(false);
    ui->CancelInstruction->setVisible(false);
    ui->OKInstruction->setVisible(false);
    ui->LoadFile->setVisible(false);
    ui->Write->setVisible(false);
    ui->LoadCode->setVisible(true);
}


void MainWindow::on_CancelInstruction_clicked()
{
    ui->InstructionInsert->clear();
    ui->InstructionInsert->setVisible(false);
    ui->InstructionShow->setVisible(false);
    ui->CancelInstruction->setVisible(false);
    ui->OKInstruction->setVisible(false);
    ui->LoadFile->setVisible(false);
    ui->Write->setVisible(false);
    ui->LoadCode->setVisible(true);
}


void MainWindow::on_DecodeInstruction_clicked()
{
    ui->ALUScreen->clear();
    ui->CUScreen->clear();
    ui->RString->setVisible(false);
    ui->RString_2->setVisible(false);
    ui->xString->setVisible(false);
    ui->XString->setVisible(false);
    ui->SString->setVisible(false);
    ui->xString_2->setVisible(false);
    ui->XString_2->setVisible(false);
    ui->YString->setVisible(false);
    ui->TString->setVisible(false);
    ui->SString_2->setVisible(false);
    ui->xString_3->setVisible(false);
    QString instruction = QString::fromStdString(cpu.getIR());
    QRegularExpression hexRegex("^[0-9A-Fa-f]+$");
    if(instruction.isEmpty() || !hexRegex.match(instruction).hasMatch() || instruction[0] == 'E' || instruction[0] == 'F')
    {
        QMessageBox::warning(this, "Input Error", "Please enter a valid hexadecimal number");
        return;
    }
    QString opcode = instruction.mid(0, 1);
    QString R = instruction.mid(1, 1);
    QString X = instruction.mid(2, 1);
    QString Y = instruction.mid(3, 1);
    ui->Opcode->setText(opcode);
    ui->R->setText(R);
    ui->X->setText(X);
    ui->Y->setText(Y);
    ui->Opcode->setAlignment(Qt::AlignCenter);
    ui->R->setAlignment(Qt::AlignCenter);
    ui->X->setAlignment(Qt::AlignCenter);
    ui->Y->setAlignment(Qt::AlignCenter);
    QString instructionEx;
    if(opcode == "1")
    {
        instructionEx = "LOAD the register " + R + " with the bit pattern found in the memory cell whose address is " + X + Y + ".";
        ui->CUScreen->setText(instructionEx);
        ui->RString->setVisible(true);
        ui->XString->setVisible(true);
        ui->YString->setVisible(true);
    }
    else if (opcode == "2")
    {
        instructionEx = "LOAD the register " + R + "  with the bit pattern " + X + Y + ".";
        ui->CUScreen->setText(instructionEx);
        ui->RString->setVisible(true);
        ui->XString->setVisible(true);
        ui->YString->setVisible(true);
    }
    else if (opcode == "3")
    {
        instructionEx = "STORE the bit pattern found in register " + R + "  in the memory cell whose address is " + X + Y + ".";
        ui->CUScreen->setText(instructionEx);
        ui->RString->setVisible(true);
        ui->XString->setVisible(true);
        ui->YString->setVisible(true);
    }
    else if (opcode == "4")
    {
        instructionEx = "MOVE the bit pattern found in register " + Y + "  to register  " + X + ".";
        ui->CUScreen->setText(instructionEx);
        ui->xString->setVisible(true);
        ui->RString_2->setVisible(true);
        ui->SString_2->setVisible(true);
    }
    else if (opcode == "5")
    {
        instructionEx = "ADD the bit patterns in registers " + X + " and " + Y + " as though they were two’s complement representations and leave the result in register " + R + ".";
        ui->ALUScreen->setText(instructionEx);
        ui->RString->setVisible(true);
        ui->SString->setVisible(true);
        ui->TString->setVisible(true);
    }
    else if (opcode == "6")
    {
        instructionEx = "ADD the bit patterns in registers " + X + " and " + Y + "  as though they represented values in floating-point notation and leave the result in register " + R + ".";
        ui->ALUScreen->setText(instructionEx);
        ui->RString->setVisible(true);
        ui->SString->setVisible(true);
        ui->TString->setVisible(true);
    }
    else if (opcode == "7")
    {
        instructionEx = "OR the bit patterns in registers " + X + " and " + Y + " and place the result in register " + R + ".";
        ui->ALUScreen->setText(instructionEx);
        ui->RString->setVisible(true);
        ui->SString->setVisible(true);
        ui->TString->setVisible(true);
    }
    else if (opcode == "8")
    {
        instructionEx = "AND the bit patterns in registers " + X + " and " + Y + " and place the result in register " + R + ".";
        ui->ALUScreen->setText(instructionEx);
        ui->RString->setVisible(true);
        ui->SString->setVisible(true);
        ui->TString->setVisible(true);
    }
    else if (opcode == "9")
    {
        instructionEx = "XOR the bit patterns in registers " + X + " and " + Y + " and place the result in register " + R + ".";
        ui->ALUScreen->setText(instructionEx);
        ui->RString->setVisible(true);
        ui->SString->setVisible(true);
        ui->TString->setVisible(true);
    }
    else if (opcode == "A")
    {
        instructionEx = "ROTATE the bit pattern in register " + R + " one bit to the right " + Y + " times. Each time, place the bit that started at the low order end at the high-order end";
        ui->ALUScreen->setText(instructionEx);
        ui->RString->setVisible(true);
        ui->SString->setVisible(true);
        ui->TString->setVisible(true);
    }
    else if (opcode == "B")
    {
        instructionEx = "JUMP to the instruction located in the memory cell at address " + X + Y + " if the bit pattern in register " + R + " is equal to the bit pattern in register number 0. Otherwise, continue with the normal sequence of execution.";
        ui->CUScreen->setText(instructionEx);
        ui->RString->setVisible(true);
        ui->XString->setVisible(true);
        ui->YString->setVisible(true);
    }
    else if (opcode == "C")
    {
        instructionEx = "HALT execution.";
        ui->CUScreen->setText(instructionEx);
        ui->xString->setVisible(true);
        ui->xString_2->setVisible(true);
        ui->xString_3->setVisible(true);
    }
    else if (opcode == "D")
    {
        instructionEx = "JUMP to the instruction located in the memory cell at address " + X + Y + " if the bit pattern in register " + R + " is greater than the bit pattern in register number 0. Otherwise, continue with the normal sequence of execution.";
        ui->CUScreen->setText(instructionEx);
        ui->RString->setVisible(true);
        ui->XString->setVisible(true);
        ui->YString->setVisible(true);
    }
}


void MainWindow::on_Execution_clicked()
{
    QRegularExpression hexRegex("^[0-9A-Fa-f]+$");

    std::string InstructionRegister = cpu.getIR();

    QString hexaFormat = QString::fromStdString(InstructionRegister);

    if(hexaFormat.isEmpty() || !hexRegex.match(hexaFormat).hasMatch() || hexaFormat[0] == 'E' || hexaFormat[0] == 'F')
    {
        QMessageBox::warning(this, "Input Error", "Please enter a valid hexadecimal number");
        return;
    }
    ir.setInstruct(InstructionRegister);
    for (int i = 0; i < 256; ++i) {
        QString memoryAddress = QString("MEM%1").arg(i, 2, 16, QChar('0')).toUpper();

        QTextEdit* memoryTextEdit = findChild<QTextEdit*>(memoryAddress);

        if (memoryTextEdit) {
            QString textValue = memoryTextEdit->toPlainText().trimmed();
            textValue.left(2).toUpper();
            memory.setMemory(i, textValue.toStdString());
        }
    }

    for (int i = 0; i < 16; ++i) {
        QString registerName = QString("REG%1").arg(i, 1, 16, QChar('0'));

        QTextEdit* registerTextEdit = findChild<QTextEdit*>(registerName);

        if (registerTextEdit) {
            QString textValue = registerTextEdit->toPlainText().trimmed();
            textValue.left(2).toUpper();
            registers.setRegister(i, textValue.toStdString());
        }
    }

    QString outputLine = ir.execute(cpu, memory, registers);
    if (!outputLine.isEmpty()) {
        updateOutputScreen(outputLine);
    }
    ui->PC->setText(QString::fromStdString(cpu.getPC()));
    ui->PC->setAlignment(Qt::AlignCenter);
    showMemory();
    showRegisters();
}


void MainWindow::on_RunOneCycle_clicked()
{
    on_FetchMemory_clicked();
    on_DecodeInstruction_clicked();
    on_Execution_clicked();
}

void MainWindow::on_RunUntilHalt_clicked()
{
    while(!ir.isHalted()){
        on_RunOneCycle_clicked();
        QEventLoop loop;
        QTimer::singleShot(500, &loop, &QEventLoop::quit);
        loop.exec();
    }
    ir.halt();
}

void MainWindow::updateOutputScreen(const QString &outputLine) {
    ui->Screen->setText(outputLine.trimmed());  // Append the text to the QTextEdit widget
}
