#include <QPainter>
#include <random>
#include "gameplay.h"

gameplay::gameplay(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("background-color:black;");//цвет окна
    resize(road_width,road_height);//размер поля
    loadQImage();
    makeblocks();
    startGame();

}

void gameplay::loadQImage()
{
    car = QImage(brick_size,brick_size,QImage::Format_ARGB32);//длина, ширина формат блока
    car.fill(Qt::yellow);
    block = QImage(brick_size,brick_size,QImage::Format_ARGB32);
    block.fill(Qt::red);
}
void gameplay::startGame()
{//создаем машину,задаем координаты верхнего левого угла каждого блока
    //машина начинает движение в центре
    for(int i = 0;i<3;i++)
    {
        x.push_back((i+3)*brick_size);
        y.push_back(road_height-brick_size);
    }
    x.push_back(4*brick_size);
    y.push_back(road_height-2*brick_size);
    for(int i = 0;i<3;i++)
    {
        x.push_back((i+3)*brick_size);
        y.push_back(road_height-3*brick_size);
    }
    x.push_back(4*brick_size);
    y.push_back(road_height-4*brick_size);

    play=true;//игра началась
    timermove = startTimer(200);//запускаем таймер, отвечает за скорость движения блоков
    timermake = startTimer(2500);
}
void gameplay::makeblocks()
{//выбираем положение блока(слева,справа,посередине)
    std::vector<int>block_start = {0,3,6};//для выбора начальной координаты рисования блоков
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> distribution(0, block_start.size() - 1);

    int index_1 = distribution(gen);
    int random_1 = block_start[index_1];

        for(int i = 0;i<3;i++)
    {
        blockx.push_back((i+random_1)*brick_size);
        blocky.push_back(0);

        };

    sum_blocks_x.push_back(blockx);
    sum_blocks_y.push_back(blocky);
    blockx.clear();
    blocky.clear();
    block_start.erase(block_start.begin() + index_1); // Удаляет элемент с индексом index_1
    int index_2 = distribution(gen);
    int random_2 = block_start[index_2];
    for(int i = 0;i<3;i++)
    {
        blockx.push_back((i+random_2)*brick_size);
        blocky.push_back(0);
    }
    sum_blocks_x.push_back(blockx);
    sum_blocks_y.push_back(blocky);
    blockx.clear();
    blocky.clear();
}
void gameplay::moveblocks()
{
    for(int i=0;i<sum_blocks_y.size();i++)
    {
        for(int y =0;y<sum_blocks_y[i].size();y++)
        {sum_blocks_y[i][y]+=brick_size;
            //если блоки вышли за границу экрана, нам они больше не нужны. Удаляем.
            //странно, но если начать удалять блоки, когда sum_blocks_y[i][y]>road_height, то начинаются глюки
            if(sum_blocks_y[i][y]>road_height+400){sum_blocks_y.erase(sum_blocks_y.begin());sum_blocks_x.erase(sum_blocks_x.begin());};
        };
    }


    //update(); // Добавляем эту строку для перерисовки виджета
}


void gameplay::paintEvent(QPaintEvent *e)//срабатывает при каждой смене координат и при их начальной установке
{
    Q_UNUSED(e);//переменную е не используем
    painting();
}

void gameplay::painting()//перед отрисовкой необходимо, чтобы все координаты уже были обозначены
{
    QPainter qp(this);
    //рисуем блоки car по координатам
    for(int i =0;i<=car_size;i++)
    {
        qp.drawImage(x[i], y[i], car);
        qp.setPen(Qt::black);//цвет линии
        qp.drawRect(x[i],y[i],car.width(),car.height());
    }
    for(int i =0;i<sum_blocks_y.size();i++)
    for(int y =0;y<sum_blocks_y[i].size();y++)
    {
        qp.drawImage(sum_blocks_x[i][y], sum_blocks_y[i][y], block);
        qp.setPen(Qt::black);//цвет линии
        qp.drawRect(sum_blocks_x[i][y],sum_blocks_y[i][y],block.width(),block.height());
    }
    if(!play){gameOver(qp);};

}
void gameplay::gameOver(QPainter &qp) {

    QString message = "Game over";
    QFont font("Courier", 15, QFont::DemiBold);
    QFontMetrics fm(font);
    int textWidth = fm.width(message);

    qp.setPen(QColor(Qt::white));
    qp.setFont(font);
    int h = height();
    int w = width();

    qp.translate(QPoint(w/2, h/2));
    qp.drawText(-textWidth/2, 0, message);
}
void gameplay::checkcollision()
{
    for(int i=0;i<sum_blocks_y.size();i++)//выбираем блоки(на практике достаточно первых 2-х)
        for(int k=0;k<sum_blocks_y[i].size();k++)//выбираем определенный блок
            for(int j=0;j<car_size;j++)//каждую деталь машины надо проверить на столкновение с блоком
            {
                if(x[j]==sum_blocks_x[i][k] && y[j]==sum_blocks_y[i][k]){play=false;killTimer(timermove);killTimer(timermake);}//если есть столкновение,игра останавливается
            }
}
void gameplay::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();//извлекаем числовое значение int key из ивента e,здесь в отличии от paintEvent, мы используем переменную e
    int i = 0;
    if (key == Qt::Key_Left)
    {for(int i =0; i<8;i++)
        {if(x[i]==0){break;};
            x[i]-= 3*brick_size;
        }}
      else if (key == Qt::Key_Right)
    {for(int i =0; i<8;i++)
        {if(x[i]==6*brick_size){break;};
            x[i]+= 3*brick_size;
        }}
    else if(key==Qt::Key_P)
    {pauseGame();}
    QWidget::keyPressEvent(e);
}
void gameplay::pauseGame() {//игра на паузу

    if (pause) {//если кнопка Р была нажата, то отключается

        timermove = startTimer(200);//запускаем таймер, отвечает за скорость движения блоков
        timermake = startTimer(2500);
        pause = false;
    }

     else {
        //если нет, ставим на паузу, убиваем таймер killTimer(timerId);
        pause = true;
        killTimer(timermove);killTimer(timermake);
    }
}
void gameplay::timerEvent(QTimerEvent *e)
{
 Q_UNUSED(e);
    if (e->timerId() == timermove)
 {checkcollision();moveblocks();}
     else if (e->timerId() == timermake)
    {makeblocks();};
    repaint();
}
