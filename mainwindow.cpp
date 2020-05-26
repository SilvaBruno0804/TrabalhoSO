#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include <QDebug>
struct processo{
    int tamanho;
    int tempo;
    int PID;
    int posInicio;
};
int vetorespacos[1024][2];
int contaposicao = 0;
processo vetorProcessos[1024];
int memoria[1024] = {0};
int contproc = 0;


void mapFreeSpacesMemory_BWFit(int memoria[]){
    bool entrou = false;
    for(int i = 0; i < 1024; i++){
        /*for(int j = i+1; j < 1024; j++){
            if(memoria[j] == 0){
                espacolivre pos;
                pos.inicio = i;
                pos.fim = j;
                vetorespacos[contaposicao] = pos;
                qInfo() << pos.inicio << " " << pos.fim;
                i = j;
                break;
            }
        }*/
        int j = i;
        while(memoria[j] == 0){
            entrou = true;
            j++;
        }
        if(entrou){
            vetorespacos[contaposicao][0] = i;
            vetorespacos[contaposicao][1] = j;
           // qInfo() << vetorespacos[contaposicao][0] << " " << vetorespacos[contaposicao][1];
            contaposicao++;
            i = j;
            entrou = false;
        }
    }
}


int bestFit(int mem[], processo proc, int num){
    mapFreeSpacesMemory_BWFit(mem);
    int menorquecabe = 4096;
    for(int i = 0; i < contaposicao; i++){
        if(vetorespacos[contaposicao-1][1] - vetorespacos[contaposicao-1][0] < menorquecabe && vetorespacos[contaposicao-1][1] - vetorespacos[contaposicao-1][0] > proc.tamanho){
                menorquecabe = vetorespacos[contaposicao-1][0];
        }
    }
    if(menorquecabe != 4096){
        for(int a = 0; a < proc.tamanho; a++){
            mem[menorquecabe + a] = num;
        }
        //contproc++;
        vetorProcessos[contproc-1] = proc;

       return menorquecabe;
    }
    else return -1;
   // qInfo() << vetorespacos[contaposicao-1][0] << " " << vetorespacos[contaposicao-1][1] << " " << contaposicao;
    //qInfo() <<  vetorespacos[contaposicao-1][1] - vetorespacos[contaposicao-1][0] << " Menor que cabe" << menorquecabe;
    return 0;
}


int firstFit(int mem[], processo proc, int num){

       for(int i = 0; i < 1024; i++){
           bool cabe = true;
           if(mem[i] == 0){
                 for(int j = 0; j < proc.tamanho; j++){
                    cabe = true;
                    if(mem[i+j] != 0){
                        cabe = false;
                        break;
                    } //endif se posicao for 1
                 } // fim for de dentro
                 if(cabe == true){
                     for(int a = 0; a < proc.tamanho; a++){
                         mem[i + a] = num;
                     }
                     return i; // retorna """pdbr"""
                 }
           } // fim se posicao 0
       } // fim for de fora

       return -1;
}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_clicked()
{
    contproc++;
    processo process;
    process.PID = contproc;
    process.tempo = ui->tempo->text().toInt();
    process.tamanho = ui->tam->text().toInt();
    int regBase = firstFit(memoria, process, process.PID);
    QMessageBox out;
    if(memoria[regBase] == process.PID){
        ui->comboBox->addItem("Processo " + QString::number(process.PID));
        process.posInicio = regBase;
        vetorProcessos[contproc-1] = process;
         out.setText("Processo " + QString::number(process.PID) +  " criado com base na posicao " + QString::number(process.posInicio));
    }
    else {
        out.setText("Não foi possível criar o processo!");
        contproc--;

    }
    out.exec();

}


void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    processo visualiza = vetorProcessos[ui->comboBox->currentIndex()];
    ui->showPID->setText(QString::number(visualiza.PID));
    ui->showTam->setText(QString::number(visualiza.tamanho));
    ui->showTempo->setText(QString::number(visualiza.tempo));
    ui->showInicio->setText(QString::number(visualiza.posInicio));
}

void MainWindow::on_pushButton_2_clicked()
{

    mapFreeSpacesMemory_BWFit(memoria);
    qInfo() << vetorespacos[contaposicao-1][0] << " " << vetorespacos[contaposicao-1][1] << " " << contaposicao;
}

/*void MainWindow::on_verificaVetor_clicked()
{
    //vetorespacos[contaposicao][0] = i;
    //vetorespacos[contaposicao][1] = j;
    qInfo() << vetorespacos[contaposicao-1][0] << " " << vetorespacos[contaposicao-1][1];
}*/

void MainWindow::on_pushButton_3_clicked()
{
    //mapFreeSpacesMemory_BWFit(memoria);
    //qInfo() << vetorespacos[contaposicao-1][0] << " " << vetorespacos[contaposicao-1][1];

}

void MainWindow::on_bestFitBtn_clicked()
{
    contproc++;
    processo process;
    process.PID = contproc;
    process.tempo = ui->tempo->text().toInt();
    process.tamanho = ui->tam->text().toInt();
    qInfo() << process.tamanho << " " << process.PID;
    QMessageBox criado;
    int verifica = bestFit(memoria, process, process.PID);
    process.posInicio = verifica;
    qInfo() << verifica;
    if(verifica != -1){
        vetorProcessos[contproc] = process;
        ui->comboBox->addItem("Processo " + QString::number(process.PID));
        criado.setText("Processo " + QString::number(process.PID) +  " criado com base na posicao " + QString::number(verifica));
        criado.exec();
    }
    else{
        criado.setText("Não foi possível criar o processo!");
        criado.exec();
    }

}
