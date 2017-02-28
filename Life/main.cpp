#include "view/main_window.h"
#include <QApplication>

class MockNotifier : public LifeGameEngine::AbstractNotifier{
public:
    virtual void notify() const override {}
};

int main(int argc, char *argv[])
{
    LifeGameEngine gameEngine(6, 6,
                              LifeGameEngine::Parameters(),
                              std::unique_ptr<LifeGameEngine::AbstractNotifier>(new MockNotifier()));
    gameEngine.set_cell_state(1, 1, LifeField::CellState::ALIVE);
    gameEngine.set_cell_state(0, 0, LifeField::CellState::ALIVE);

    QApplication app(argc, argv);
    MainWindow window(&gameEngine);
    window.show();
    return app.exec();
}
