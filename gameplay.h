#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#include <QWidget>
#include <QKeyEvent>
#include <QTime>

class gameplay: public QWidget
{
public:
    gameplay(QWidget *parent =0);//конструктор


protected:
    void paintEvent(QPaintEvent *e);//причина первого срабатывания QPaintEvent - window.show();
    void timerEvent(QTimerEvent *e);//срабатывает при смене таймера, по сути игровой процесс
    void keyPressEvent(QKeyEvent *e);// ловит сигналы от клавиатуры


private:
    QImage car;
    QImage block;

    static const int brick_size = 20;//размер кирпича из которого будет состоять все объекты
    //окно должно быть в ширину = 4 ширины машины,чтобы было 4 полосы
    static const int road_height = 600;//длина дороги
    static const int road_width = 180;//ширина дороги
    static const int car_size = 8;//количество блоков в машине
    //текущие координаты машины
    std::vector <int> x;
    std::vector<int>y;
    //текущие координаты блоков
    std::vector<std::vector<int>>  sum_blocks_x;
    std::vector<std::vector<int>>  sum_blocks_y;
    std::vector<int>blockx;
    std::vector<int>blocky;
     int timermove;//счетчик таймера движения блоков
    int timermake;//счетчик таймера создания блоков
    void loadQImage();//заполняем кирпичи для моделей
    void startGame();//начинаем гонку
    void painting();//рисуем модели
    void makeblocks();//начальные координаты блоков
    void moveblocks();//движение блоков
    void gameOver(QPainter &qp);
    void checkcollision();//проверяем столкновение с ближайшими 2-мя блоками
    bool play = false;//игра идет или нет
    bool pause;//показатель паузы игры
    void pauseGame();//ставит на паузу

};

#endif // GAMEPLAY_H
