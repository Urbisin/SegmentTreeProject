#include "MainWindow.h"
#include <QGraphicsEllipseItem>
#include <QVBoxLayout>
#include <cmath>
#include <iostream>
#include <QRegularExpressionValidator>

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), segTree(nullptr) {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    inputField = new QLineEdit(this);
    inputField->setPlaceholderText("Enter numbers separated by commas (e.g., 1,3,5,7,9,11)");
    QRegularExpression regExp("[0-9,]*");
    inputField->setValidator(new QRegularExpressionValidator(regExp, this));

    updateIndexField = new QLineEdit(this);
    updateIndexField->setPlaceholderText("Index to update");
    updateValueField = new QLineEdit(this);
    updateValueField->setPlaceholderText("New value");

    generateButton = new QPushButton("Generate Segment Tree", this);
    connect(generateButton, &QPushButton::clicked, this, &MainWindow::onGenerateTree);

    updateButton = new QPushButton("Update Segment Tree", this);
    connect(updateButton, &QPushButton::clicked, this, &MainWindow::onUpdateTree);

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);

    layout->addWidget(inputField);
    layout->addWidget(generateButton);
    layout->addWidget(updateIndexField);
    layout->addWidget(updateValueField);
    layout->addWidget(updateButton);
    layout->addWidget(view);
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow() {
    delete segTree;
}

void MainWindow::onGenerateTree() {
    QString inputText = inputField->text();
    QStringList inputList = inputText.split(",", Qt::SkipEmptyParts);
    vector<int> inputVector;

    for (const QString& number : inputList) {
        inputVector.push_back(number.toInt());
    }

    if (segTree) {
        delete segTree;
    }

    segTree = new SegmentTree<int>(inputVector);
    auto tree_structure = segTree->getTreeStructure();

    // Imprimir la estructura del árbol
    for (const auto& node : tree_structure) {
        int start, end, sum, maxVal, minVal;
        std::tie(start, end, sum, maxVal, minVal) = node;
        std::cout << "Start: " << start << ", End: " << end << ", Sum: " << sum << ", Max: " << maxVal << ", Min: " << minVal << std::endl;
    }

    drawTree(tree_structure, inputVector);
}

void MainWindow::onUpdateTree() {
    int idx = updateIndexField->text().toInt();
    int val = updateValueField->text().toInt();

    if (segTree) {
        segTree->update(idx, val);
        auto tree_structure = segTree->getTreeStructure();
        drawTree(tree_structure, vector<int>()); // Pasar el vector de entrada solo si necesitas redibujar los nodos de entrada
    }
}

void MainWindow::drawTree(const std::vector<std::tuple<int, int, int, int, int>>& tree_structure, const std::vector<int>& inputVector) {
    scene->clear();

    if (tree_structure.empty()) return;

    int nodeRadius = 30;
    int xOffset = 70;
    int yOffset = 70;
    int depth = std::ceil(std::log2(tree_structure.size() + 1));
    int baseY = yOffset * depth;

    std::vector<QPointF> nodePositions(tree_structure.size());

    // Calcular posiciones de nodos hojas
    int leafIndex = 0;
    for (int i = 0; i < tree_structure.size(); ++i) {
        const auto& [start, end, sum, maxVal, minVal] = tree_structure[i];
        if (start == end) {
            int x = xOffset * (leafIndex + 1);
            int y = baseY;
            nodePositions[i] = QPointF(x, y);
            leafIndex++;
        }
    }

    // Calcular posiciones de nodos internos
    for (int i = tree_structure.size() - 1; i >= 0; --i) {
        const auto& [start, end, sum, maxVal, minVal] = tree_structure[i];
        if (start != end) {
            int leftChildIdx = 2 * i + 1;
            int rightChildIdx = 2 * i + 2;
            if (leftChildIdx < tree_structure.size() && rightChildIdx < tree_structure.size()) {
                int x = (nodePositions[leftChildIdx].x() + nodePositions[rightChildIdx].x()) / 2;
                int level = std::floor(std::log2(i + 1));
                int y = baseY - (depth - level) * yOffset;
                nodePositions[i] = QPointF(x, y);
            }
        }
    }

    // Dibujar nodos y líneas
    for (int i = 0; i < tree_structure.size(); ++i) {
        const auto& [start, end, sum, maxVal, minVal] = tree_structure[i];
        int x = nodePositions[i].x();
        int y = nodePositions[i].y();
        drawNode(x, y, nodeRadius, QString("[%1,%2]\nSum: %3\nMax: %4\nMin: %5").arg(start).arg(end).arg(sum).arg(maxVal).arg(minVal), true);

        if (start != end) {
            int leftChildIdx = 2 * i + 1;
            int rightChildIdx = 2 * i + 2;
            if (leftChildIdx < tree_structure.size()) {
                drawLine(x, y, nodePositions[leftChildIdx].x(), nodePositions[leftChildIdx].y());
            }
            if (rightChildIdx < tree_structure.size()) {
                drawLine(x, y, nodePositions[rightChildIdx].x(), nodePositions[rightChildIdx].y());
            }
        }
    }
}

void MainWindow::drawNode(int x, int y, int radius, const QString& text, bool isQuery) {
    QGraphicsEllipseItem* ellipse = scene->addEllipse(x - radius, y - radius, radius * 2, radius * 2, QPen(), QBrush(isQuery ? Qt::white : Qt::yellow));
    QGraphicsTextItem* textItem = scene->addText(text);
    textItem->setDefaultTextColor(isQuery ? Qt::red : Qt::black);
    textItem->setPos(x - radius / 2, y - radius / 2);
}

void MainWindow::drawLine(int x1, int y1, int x2, int y2) {
    scene->addLine(x1, y1, x2, y2, QPen(Qt::black));
}
