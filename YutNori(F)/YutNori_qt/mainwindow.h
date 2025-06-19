#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "button_reader.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // for IPC - button
    void selectPiece(int index);
    void selectYutResult();      // ← 요기!
    void moveSelectedPiece(); 
    void startButtonReceiver();
    
public slots:
    void handleButtonInput(char input);

private:
    Ui::MainWindow *ui;
    int selectedPieceIndex = -1;  // 현재 선택된 말 인덱스 (-1은 선택 없음)

};
#endif // MAINWINDOW_H
