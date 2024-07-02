#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QLineEdit>
#include <QPushButton>
#include <vector>
#include <tuple>
#include "SegmentTree.h"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onGenerateTree();

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QLineEdit *inputField;
    QPushButton *generateButton;
    SegmentTree<int> *segTree;

    void drawTree(const std::vector<std::tuple<int, int, int>>& tree_structure, const std::vector<int>& inputVector);
    void drawNode(int x, int y, int radius, const QString& text);
    void drawLine(int x1, int y1, int x2, int y2);
};

#endif // MAINWINDOW_H
