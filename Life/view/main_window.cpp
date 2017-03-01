#include "main_window.h"

#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QTextStream>

MainWindow::MainWindow(QWidget * parent)
        : QMainWindow(parent),
          signal_notifier(new SignalNotifier()),
          game_engine(new LifeGameEngine(default_cols, default_rows, signal_notifier.get())),
          field_scroll_area(new QScrollArea()) {

    create_actions();
    create_menus();

    setCentralWidget(field_scroll_area);

    field_scroll_area->setVisible(true);
    field_scroll_area->setWidgetResizable(true);
    field_display = new FieldDisplay(game_engine.get(), default_edge_size);
    field_scroll_area->setWidget(field_display);
    field_scroll_area->setPalette(field_display->palette());

    connect_all();

    setWindowTitle(tr("Life"));
    resize(500, 500);
}

void MainWindow::create_actions() {
    new_field_action = new QAction(tr("&New"), this);
    open_field_action = new QAction(tr("&Open"), this);
    save_field_action = new QAction(tr("&Save"), this);
    exit_action = new QAction(tr("E&xit"), this);
    next_step_action = new QAction(tr("Next"), this);
    run_action = new QAction(tr("&Run"), this);
    toggle_impacts_action = new QAction(tr("Impact"), this);
    clear_field_action = new QAction(tr("Clear"), this);
}

void MainWindow::create_menus() {
    QMenu * file_menu = new QMenu(tr("&File"));
    file_menu->addAction(new_field_action);
    file_menu->addAction(open_field_action);
    file_menu->addAction(save_field_action);
    file_menu->addAction(exit_action);

    QMenu * edit_menu = new QMenu(tr("&Edit"));
    edit_menu->addAction(next_step_action);
    edit_menu->addAction(run_action);
    edit_menu->addAction(toggle_impacts_action);
    edit_menu->addAction(clear_field_action);

    menuBar()->addMenu(file_menu);
    menuBar()->addMenu(edit_menu);
}

void MainWindow::connect_all() {
    connect(new_field_action, SIGNAL(triggered()),
            this, SLOT(new_field()));
    connect(open_field_action, SIGNAL(triggered()),
            this, SLOT(open_field()));
    connect(save_field_action, SIGNAL(triggered()),
            this, SLOT(save_field()));
    connect(exit_action, SIGNAL(triggered()),
            this, SLOT(exit()));
    connect(next_step_action, SIGNAL(triggered()),
            this, SLOT(next_step()));
    connect(run_action, SIGNAL(triggered()),
            this, SLOT(run()));
    connect(clear_field_action, SIGNAL(triggered()),
            this, SLOT(clear_field()));

    connect(toggle_impacts_action, SIGNAL(triggered()),
            field_display, SLOT(toggle_impacts()));
    connect(signal_notifier.get(), SIGNAL(notification()),
            field_display, SLOT(model_changed()));
}

void MainWindow::new_field() {}

void MainWindow::open_field() {
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
    if (!file_name.isEmpty()) {
        open_field_file(file_name);
    }
}

void MainWindow::save_field() {}

void MainWindow::exit() {}

void MainWindow::next_step() {
    game_engine->next_step();
}

void MainWindow::run() {}

void MainWindow::clear_field() {
    game_engine->clear();
}

bool MainWindow::open_field_file(const QString & filename) {
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        bool ok = false;
        QTextStream input(&file);

        //  Read cols and rows count.
        QString line = input.readLine();
        if (line.isNull()) {
            return false;
        }
        QStringList line_parts = line.split(' ');
        uint32_t cols = line_parts[0].toUInt(&ok);
        if (!ok) {
            return false;
        }
        uint32_t rows = line_parts[1].toUInt(&ok);
        if (!ok || (line_parts.size() > 2 && !line_parts[2].startsWith("//"))) {
            return false;
        }

        // Read cell separator width.
        line = input.readLine();
        if (line.isNull()) {
            return false;
        }
        line_parts = line.split(' ');
        uint32_t cell_separator_width = line_parts[0].toUInt(&ok);
        if (!ok || (line_parts.size() > 1 && !line_parts[1].startsWith("//"))) {
            return false;
        }

        // Read cell edge size.
        line = input.readLine();
        if (line.isNull()) {
            return false;
        }
        line_parts = line.split(' ');
        uint32_t cell_edge_size = line_parts[0].toUInt(&ok);
        if (!ok || (line_parts.size() > 1 && !line_parts[1].startsWith("//"))) {
            return false;
        }

        // Read alive cell count.
        line = input.readLine();
        if (line.isNull()) {
            return false;
        }
        line_parts = line.split(' ');
        uint32_t alive_cell_count = line_parts[0].toUInt(&ok);
        if (!ok || (line_parts.size() > 1 && !line_parts[1].startsWith("//"))) {
            return false;
        }

        std::unique_ptr<SignalNotifier> notifier;
        std::unique_ptr<LifeGameEngine> engine;
        std::unique_ptr<FieldDisplay> display;
        try {
            notifier = std::unique_ptr<SignalNotifier>(new SignalNotifier());
            engine = std::unique_ptr<LifeGameEngine>(new LifeGameEngine(cols, rows, notifier.get()));
            display = std::unique_ptr<FieldDisplay>(new FieldDisplay(engine.get(), cell_edge_size));
            connect(notifier.get(), SIGNAL(notification()),
                    display.get(), SLOT(model_changed()));
        }
        catch (std::bad_alloc & e) {
            return false;
        }

        // Read alive cell positions.
        for (uint32_t i = 0; i < alive_cell_count; ++i) {
            line = input.readLine();
            if (line.isNull()) {
                return false;
            }
            line_parts = line.split(' ');
            uint32_t row = line_parts[1].toUInt(&ok);
            if (!ok || row >= rows || (line_parts.size() > 2 && !line_parts[2].startsWith("//"))) {
                return false;
            }
            uint32_t col = line_parts[0].toUInt(&ok);
            if (!ok || col >= cols - (row % 2)) {
                return false;
            }

            engine->set_cell(col, row);
        }

        if (!input.atEnd()) {
            return false;
        }

        disconnect(toggle_impacts_action, SIGNAL(triggered()),
                field_display, SLOT(toggle_impacts()));
        disconnect(signal_notifier.get(), SIGNAL(notification()),
                field_display, SLOT(model_changed()));

        signal_notifier = std::move(notifier);
        game_engine = std::move(engine);
        field_display = display.release();
        field_scroll_area->setWidget(field_display);

        connect(toggle_impacts_action, SIGNAL(triggered()),
                field_display, SLOT(toggle_impacts()));

        return true;
    }
    return false;
}
