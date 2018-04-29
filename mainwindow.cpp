#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    addressRequest = new QNetworkAccessManager();
    connect(addressRequest, SIGNAL(finished(QNetworkReply*)),
            SLOT(getAddressFinished(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getAddressFinished(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll();

    qDebug() << QString::fromUtf8(bytes.data(), bytes.size());

    QXmlStreamReader xml;
    xml.addData(bytes);

    while(!xml.atEnd())
    {
        if (xml.isStartElement())
        {
            QString name = xml.name().toString();
            qDebug() << name;

            if (name == "formatted_address")
            {
                QString text = xml.readElementText();
                qDebug() << "Address:" << text;
                return;
            }
        }

        xml.readNext();
    }

    if (xml.hasError())
    {
        qDebug() << "Error loading XML:" << xml.errorString();
        return;
    }

    qDebug() << "No result.";
}

void MainWindow::on_pushButton_clicked()
{
    QString latitude = ui->lineEdit->text();
    QString longitude = ui->lineEdit_2->text();

    QNetworkRequest request;
    request.setUrl(QUrl("http://maps.googleapis.com/maps/api/geocode/xml?latlng="
                        + latitude + "," + longitude + "&sensor=false"));
    addressRequest->get(request);
}
