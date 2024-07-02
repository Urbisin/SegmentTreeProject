#include "mainwindow.h"
#include <QGraphicsEllipseItem>
#include <QVBoxLayout>
#include <cmath>
#include <QRegularExpressionValidator>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), segTree(nullptr) {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    inputField = new QLineEdit(this);
    inputField->setPlaceholderText("Enter numbers separated by commas (e.g., 1,3,5,7,9,11)");
    QRegularExpression regExp("[0-9,]*");
    inputField->setValidator(new QRegularExpressionValidator(regExp, this));

    generateButton = new QPushButton("Generate Segment Tree", this);
    connect(generateButton, &QPushButton::clicked, this, &MainWindow::onGenerateTree);

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);

    layout->addWidget(inputField);
    layout->addWidget(generateButton);
    layout->addWidget(view);
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow() {
    delete segTree;
}

void MainWindow::onGenerateTree() {
    QString inputText = inputField->text();
    QStringList inputList = inputText.split(",", Qt::SkipEmptyParts);
    std::vector<int> inputVector;

    for (const QString& number : inputList) {
        inputVector.push_back(number.toInt());
    }

    if (segTree) {
        delete segTree;
    }

    segTree = new SegmentTree<int>(inputVector);
    auto tree_structure = segTree->getTreeStructure();
    drawTree(tree_structure, inputVector);
}

void MainWindow::drawTree(const std::vector<std::tuple<int, int, int>>& tree_structure, const std::vector<int>& inputVector) {
    scene->clear();

    if (tree_structure.empty()) return;

    int nodeRadius = 20;
    int xOffset = 70;
    int yOffset = 70;
    int depth = std::log2(tree_structure.size() + 1);
    int baseY = yOffset * depth;

    std::vector<QPointF> nodePositions(tree_structure.size());
    std::vector<int> levelNodesCount(depth + 1, 0);

    // Dibujar lista de entrada en la base del árbol
    for (size_t i = 0; i < inputVector.size(); ++i) {
        int x = xOffset * (i * 2 + 1);
        int y = baseY;
        drawNode(x, y, nodeRadius, QString::number(inputVector[i]));
        nodePositions[i] = QPointF(x, y);
    }

    // Dibujar nodos del árbol
    for (size_t i = 0; i < tree_structure.size(); ++i) {
        if (i < inputVector.size()) continue; // Saltar los nodos base ya dibujados

        const auto& [start, end, sum] = tree_structure[i];

        int level = std::log2(i + 1) - 1;
        int posInLevel = levelNodesCount[level]++;

        int x = xOffset * ((posInLevel * 2 + 1) * (depth - level));
        int y = yOffset * level;

        drawNode(x, y, nodeRadius, QString("[%1,%2]\nSum:%3").arg(start).arg(end).arg(sum));
        nodePositions[i] = QPointF(x, y);

        if (i > 0) {
            int parentIdx = (i - 1) / 2;
            drawLine(nodePositions[parentIdx].x(), nodePositions[parentIdx].y() + nodeRadius, x, y - nodeRadius);
        }
    }
}

void MainWindow::drawNode(int x, int y, int radius, const QString& text) {
    scene->addEllipse(x - radius, y - radius, radius * 2, radius * 2);
    QGraphicsTextItem* textItem = scene->addText(text);
    textItem->setPos(x - radius, y - radius / 2);
}

void MainWindow::drawLine(int x1, int y1, int x2, int y2) {
    scene->addLine(x1, y1, x2, y2);
}
