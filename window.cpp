#include "window.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    colorPen = new QColor(Qt::red);

    this->setMinimumSize(600, 400);


    QVBoxLayout *mainLayout = new QVBoxLayout();
    QSplitter *mainSplit = new QSplitter(Qt::Horizontal);

    QHBoxLayout *headLayout = new QHBoxLayout();

    QGroupBox *opcGrp1 = new QGroupBox(tr("Tipo de linea que desea usar"));
    QFormLayout *opcForm1 = new QFormLayout();
    opcGrp1->setLayout(opcForm1);

    radio[0] = new QRadioButton(tr("Segmentos de lineas rectas"));
    radio[0]->setChecked(true);
    opcForm1->addRow(radio[0]);

    radio[1] = new QRadioButton(tr("Linea continua a mano"));
    opcForm1->addRow(radio[1]);

    headLayout->addWidget(opcGrp1);
    mainLayout->addLayout(headLayout);

    QGroupBox *opcGrp2 = new QGroupBox(tr("Controles"));
    QFormLayout *opcForm2 = new QFormLayout();
    opcGrp2->setLayout(opcForm2);

    //opcForm2->addRow(btnUndo);

    btnColor = new QPushButton("Color de pincel: ");
    lblColor = new QLabel(colorPen->name());
    lblColor->setPalette(QPalette(colorPen->rgb()));
    lblColor->setAutoFillBackground(true);

    opcForm2->addRow(lblColor, btnColor);
    connect(btnColor, SIGNAL(clicked()), this, SLOT(on_btnColor_clicked()));
    headLayout->addWidget(opcGrp2);

    mainSplit->setChildrenCollapsible(false);

    for(int i=0; i<2; ++i) {
        imgs[i] = new QImage();

        view[i] = new GraphicsView();
        view[i]->enableDrawing(false);

        scen[i] = new QGraphicsScene();

        btnUndo[i] = new QPushButton("Deshacer ultimo trazo");
        btnUndo[i]->setStyleSheet("padding: 10px; ");
        connect(btnUndo[i], SIGNAL(clicked()), this, SLOT(on_btnUndo_clicked()));

        btnClear[i] = new QPushButton("Limpiar lienzo");
        btnClear[i]->setStyleSheet("padding: 10px; ");
        connect(btnClear[i], SIGNAL(clicked()), this, SLOT(on_btnClear_clicked()));

        btnOpen[i] = new QPushButton(tr("Cargar imagen"));
        btnOpen[i]->setStyleSheet("padding: 10px; ");
        connect(btnOpen[i], SIGNAL(clicked()), this, SLOT(on_btnOpen_clicked()));

        imageContainer[i] = new QVBoxLayout();        

//        txtRuta[i] = new QLineEdit();
//        txtRuta[i]->setDisabled(true);
//        imageContainer[i]->addWidget(txtRuta[i]);

        imageContainer[i]->addWidget(btnUndo[i]);
        imageContainer[i]->addWidget(btnClear[i]);
        imageContainer[i]->addWidget(view[i]);
        imageContainer[i]->addWidget(btnOpen[i]);

        frame[i] = new QFrame();
        frame[i]->setLayout(imageContainer[i]);

        mainSplit->addWidget(frame[i]);
        view[i]->setScene(scen[i]);
    }

    mainLayout->addWidget(mainSplit);
    this->setLayout(mainLayout);

    diaImage = new QFileDialog(this);
    diaImage->setFileMode(QFileDialog::ExistingFile);
    diaImage->setNameFilter(tr("Images (*.png *.jpg *bmp)"));
    diaImage->setViewMode(QFileDialog::Detail);

    diaColor = new QColorDialog(this);
    diaColor->setCurrentColor(colorPen->rgb());
}

MainWindow::~MainWindow() {
//    delete imageDialog;

//    delete[] imgs;
//    delete[] scen;
//    delete[] view;
//    delete[] btnOpen;

//    delete[] frame;
//    delete[] imageContainer;
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}

void MainWindow::on_btnOpen_clicked() {
    bool pos = (sender() == btnOpen[RIGTH]);
    LoadImage(pos);
}


void MainWindow::on_btnClear_clicked() {
    bool pos = (sender() == btnClear[RIGTH]);
    CleanCanvas(pos);
}

void MainWindow::on_btnUndo_clicked() {
    bool pos = (sender() == btnUndo[RIGTH]);
    UndoLineCanvas(pos);
}

void MainWindow::on_btnColor_clicked() {
    if(!diaColor->exec()) return;

    *colorPen = diaColor->selectedColor();
    lblColor->setText(colorPen->name());
    lblColor->setPalette(QPalette(colorPen->rgb()));
    GraphicsView::colorDrawing = colorPen;
}

void MainWindow::LoadImage(bool pos) {
    QStringList fileNames;

    if (!diaImage->exec()) return;

    fileNames = diaImage->selectedFiles();

    QString imagePath = fileNames.first();

    QSize sizecont = view[pos]->size();
    sizecont -= QSize(5, 5);

    imgs[pos]->load(imagePath);

    scen[pos]->clear();
    scen[pos]->setSceneRect(0, 0, sizecont.width(), sizecont.height());
    scen[pos]->addPixmap(QPixmap::fromImage(imgs[pos]->scaled(sizecont, Qt::KeepAspectRatio)));    

    view[pos]->enableDrawing(true);
}

void MainWindow::CleanCanvas(bool pos) {
    view[pos]->cleanLines();
}

void MainWindow::UndoLineCanvas(bool pos) {
    view[pos]->undoLastLine();
}
