#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QTime>
#include <sstream>
#include <QDebug>

#define MIN_DISCS_CNT 5
#define MAX_DISCS_CNT 5

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    algorithm = new ConvexHullAlgorithm(this,discs);

    ui->setupUi(this);
    connect(ui->run_btn, SIGNAL(clicked()), this, SLOT(run()));
    connect(ui->clear_btn, SIGNAL(clicked()), this, SLOT(clear()));
    connect(ui->load_btn, SIGNAL(clicked()), this, SLOT(load()));
    connect(ui->random_btn, SIGNAL(clicked()), this, SLOT(random()));
    connect(ui->manual_btn, SIGNAL(clicked()), this, SLOT(manual()));
    connect(ui->info_btn, SIGNAL(clicked()), this, SLOT(info()));

    connect(algorithm, SIGNAL(addEgde(Edge*)), ui->graphicsView, SLOT(addEgde(Edge*)));
    connect(algorithm, SIGNAL(addLp(Edge*)), ui->graphicsView, SLOT(addLp(Edge*)));
    connect(algorithm, SIGNAL(addLq(Edge*)), ui->graphicsView, SLOT(addLq(Edge*)));
    connect(algorithm, SIGNAL(addLstar(Edge*)), ui->graphicsView, SLOT(addLstar(Edge*)));
    connect(algorithm, SIGNAL(delayUpdateView()), ui->graphicsView, SLOT(delayUpdateView()));

    connect(algorithm, SIGNAL(highlightDisc(Disc*)), ui->graphicsView, SLOT(highlightDisc(Disc*)));
    connect(algorithm, SIGNAL(highlightEdge(Edge*)), ui->graphicsView, SLOT(highlightEdge(Edge*)));
    connect(algorithm, SIGNAL(highlightParts(ConvexHullList*,ConvexHullList*)), ui->graphicsView, SLOT(highlightParts(ConvexHullList*,ConvexHullList*)));

    connect(algorithm, SIGNAL(unhighlightDisc(Disc*)), ui->graphicsView, SLOT(unhighlightDisc(Disc*)));
    connect(algorithm, SIGNAL(unhighlightEdge(Edge*)), ui->graphicsView, SLOT(unhighlightEdge(Edge*)));
    connect(algorithm, SIGNAL(unhighlightParts(ConvexHullList*,ConvexHullList*)), ui->graphicsView, SLOT(unhighlightParts(ConvexHullList*,ConvexHullList*)));

    connect(algorithm, SIGNAL(removeEdge(Edge*)), ui->graphicsView, SLOT(removeEdge(Edge*)));
    connect(algorithm, SIGNAL(removeEdges(ConvexHullEdges*)), ui->graphicsView, SLOT(removeEdges(ConvexHullEdges*)));

}

void MainWindow::run()
{
    ui->run_btn->setEnabled(false);
    ui->clear_btn->setEnabled(false);
    ui->manual_btn->setEnabled(false);
    ui->random_btn->setEnabled(false);
    ui->load_btn->setEnabled(false);


    algorithm->setDiscs(discs);
    for (int i = 0; i < discs.length(); ++i)
        qDebug() << discs[i]->to_qstring();
    result = algorithm->find(0,discs.length() - 1);
    ui->clear_btn->setEnabled(true);
}

void MainWindow::clear()
{

    for (int i = 0; i < result.edges.length(); ++i)
    {
        ui->graphicsView->getScene()->removeItem(result.edges[i]);
        delete result.edges[i];
    }

    for (int i = 0; i < discs.length(); ++i)
    {
        ui->graphicsView->getScene()->removeItem(discs[i]);
        delete discs[i];
    }

    discs.clear();


    ui->graphicsView->getScene()->update();
    ui->random_btn->setEnabled(true);
    ui->load_btn->setEnabled(true);
    ui->clear_btn->setEnabled(false);
}

void MainWindow::info()
{
    QMessageBox::information(this, tr("Uputstvo"),
     "Program za generisanje konveksnog omotača krugova.\n\n"
     "Implementirani algoritam je objavio David Rappaport 1992. god. "
     "u časopisu Computational Geometry http://www.sciencedirect.com/science/article/pii/092577219290015K \n\n"
     "Algoritam koristi tehniku zavadi(podeli) pa vladaj \n"
     "Složenost O(nlogn), n - broj krugova \n\n"

     "Postoje 2 načina za unos krugova: (ineraktivni unos u izradi) \n"
     "1) Generisanjem nekoliko (od 5 do 15) slučajnih krugova\n"
     "2) Krugovi se učitavaju iz falja (format: u svakom redu se nalaze 4 realna broja razdvojena razmakom "
     " koje predstavljaju koordinate centra i tačke na kružnici) \n");
}
void MainWindow::load()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open file"), ".", "All files (*.*);;Text files (*.txt)");
    QFile inputFile(filename);

    if (!inputFile.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox messageBox;
        messageBox.critical(0, "Greska!", "Fajl nije pronađen!");
        messageBox.setFixedSize(200, 200);
        return;
    }

    QTextStream in(&inputFile);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        std::istringstream lstream(line.toStdString());
        double x1, y1, x2, y2;
        lstream >> x1 >> y1 >> x2 >> y2;
        discs.push_back(new Disc(QPointF(x1,y1),QPointF(x2,y2)));
    }
    inputFile.close();

    for(int i = 0; i < discs.size() ; ++i)
        ui->graphicsView->getScene()->addItem(discs[i]);
    ui->graphicsView->getScene()->update();

    ui->run_btn->setEnabled(true);
}

int MainWindow::randomInt(int min,int max)
{
    return min + qrand() % (max - min + 1);
}

void MainWindow::random()
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    int cnt = randomInt(MIN_DISCS_CNT, MAX_DISCS_CNT);
    int w = ui->graphicsView->getScene()->sceneRect().width() - 50;
    int h = ui->graphicsView->getScene()->sceneRect().height() - 50;

    int min_x = -w/2;
    int min_y = -h/2;

    while(cnt > 0)
    {
        int dx = randomInt(0,w);
        int dy = randomInt(0,h);
        int x1 = min_x + dx;
        int y1 = min_y + dy;

        int min_rx = dx < w - dx ? dx : w - dx;
        int min_ry = dy < h - dy ? dy : h - dy;
        int r = 5 + (min_rx < min_ry ? min_rx : min_ry);
        discs.push_back(new Disc(QPointF(x1,y1),QPointF(x1,y1 + r)));
        cnt--;
    }
    for(int i = 0; i < discs.size() ; ++i)
        ui->graphicsView->getScene()->addItem(discs[i]);
    ui->graphicsView->getScene()->update();

    ui->run_btn->setEnabled(true);
}
void MainWindow::manual()
{

}

MainWindow::~MainWindow()
{
    delete ui;
}
