#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class Registers {
private:
    std::string reg[16];
    const int size;
public:
    Registers();
    std::string getRegister(int address);
    void setRegister(int address, const std::string& val);
    void clearRegister();
};

class Memory{
private:
    std::string mem[256];
    const int size;
public:
    Memory();
    std::string getMemory(int address);
    void setMemory(int address, const std::string& val);
    void clearMemory();
};

class CPU{
private:
    Registers registers;
    std::string pc = "0A";
    std::string ir;
public:
    void setPC(std::string val);
    std::string getPC();
    std::string getIR();
    std::string fetch(Memory& memory);
    void decode();
    void execute();
    bool validateInstruction();
    void runOneCycle();
    void runUntilHalt();
    int hex_string_to_int(const QString &hexStr);
    QString int_to_hex_string(int value);
};

class Instruction{
private:
    std::string instruct;
    Registers registers;
    QString outputLine;
    bool haltFlag = false;
public:
    void setInstruct(const std::string& code);
    void clearOutput();
    void load(CPU& cpu, Memory& memory, Registers& registers, const std::string& R, const std::string& XY);
    void copy(CPU& cpu, Memory& memory, Registers& registers, const std::string& R, const std::string& XY);
    void store(CPU& cpu, Memory& memory, Registers& registers, const std::string& R, const std::string& XY);
    void move(CPU& cpu, Registers& registers, const std::string& R, const std::string& S);
    void jump(CPU& cpu, Registers& registers, const std::string& R, const std::string& XY);
    void jumpg(CPU& cpu, Registers& registers, const std::string& R, const std::string& XY);
    void halt();
    bool isHalted() const;
    void rotateRight(CPU& cpu, Registers& registers, const std::string& R, const std::string& S);
    void andOperation(CPU& cpu, Registers& registers, const std::string& R, const std::string& S, const std::string& T);
    void orOperation(CPU& cpu, Registers& registers, const std::string& R, const std::string& S, const std::string& T);
    void xorRotation(CPU& cpu, Registers& registers, const std::string& R, const std::string& S, const std::string& T);
    void addOperationTwo(CPU& cpu, Registers& registers, const std::string& R, const std::string& S, const std::string& T);
    void addOperationFloating(CPU& cpu, Registers& registers, const std::string& R, const std::string& S, const std::string& T);
    QString execute(CPU& cpu, Memory& memory, Registers& registers);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateOutputScreen(const QString &text);

private slots:
    void on_HelpButton_clicked();

    void on_CancelMenu_clicked();

    void on_FetchMemory_clicked();

    void on_ClearRegister_clicked();

    void on_ClearMemory_clicked();

    void showMemory();

    void showRegisters();

    void on_LoadCode_clicked();

    void on_LoadFile_clicked();

    void on_Write_clicked();

    void on_OKInstruction_clicked();

    void on_CancelInstruction_clicked();

    void on_DecodeInstruction_clicked();

    void on_Execution_clicked();

    void on_RunOneCycle_clicked();

    void on_RunUntilHalt_clicked();

private:
    Ui::MainWindow *ui;
    Registers registers;
    Memory memory;
    CPU cpu;
    Instruction ir;
};

#endif // MAINWINDOW_H
