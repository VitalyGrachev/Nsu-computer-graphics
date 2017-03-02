#include "main_window.h"

#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QTextStream>
#include <iostream>

MainWindow::MainWindow(QWidget * parent)
        : QMainWindow(parent),
          signal_notifier(new SignalNotifier()),
          game_engine(new LifeGameEngine(default_cols, default_rows, signal_notifier.get())),
          field_scroll_area(new QScrollArea(this)),
          timer(new QTimer(this)),
          is_running(false) {

    create_actions();
    create_menus();
    timer->setInterval(timer_interval_msec);

    setCentralWidget(field_scroll_area);

    field_scroll_area->setVisible(true);
    field_scroll_area->setWidgetResizable(true);
    field_display = new FieldDisplay(game_engine.get(), default_edge_size, this);
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
    run_action->setCheckable(true);
    toggle_impacts_action = new QAction(tr("Impact"), this);
    clear_field_action = new QAction(tr("Clear"), this);

    set_replace_mode_action = new QAction(tr("Replace"), this);
    set_xor_mode_action = new QAction(tr("XOR"), this);
    mode_action_group = new QActionGroup(this);
    mode_action_group->addAction(set_replace_mode_action);
    mode_action_group->addAction(set_xor_mode_action);
    set_replace_mode_action->setCheckable(true);
    set_xor_mode_action->setCheckable(true);
    set_replace_mode_action->setChecked(true);
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
    edit_menu->addSeparator()->setText(tr("Mode"));
    edit_menu->addAction(set_replace_mode_action);
    edit_menu->addAction(set_xor_mode_action);

    menuBar()->addMenu(file_menu);
    menuBar()->addMenu(edit_menu);
}

void MainWindow::connect_all() {
    connect(new_field_action, &QAction::triggered,
            this, &MainWindow::new_field);
    connect(open_field_action, &QAction::triggered,
            this, &MainWindow::open_field);
    connect(save_field_action, &QAction::triggered,
            this, &MainWindow::save_field);
    connect(exit_action, &QAction::triggered,
            this, &MainWindow::exit);

    connect(next_step_action, &QAction::triggered,
            this, &MainWindow::next_step);
    connect(run_action, &QAction::triggered,
            this, &MainWindow::toggle_run);
    connect(clear_field_action, &QAction::triggered,
            this, &MainWindow::clear_field);

    connect(toggle_impacts_action, &QAction::triggered,
            field_display, &FieldDisplay::toggle_impacts);
    connect(signal_notifier.get(), &SignalNotifier::notification,
            field_display, &FieldDisplay::model_changed);
    connect(set_xor_mode_action, &QAction::triggered,
            field_display, &FieldDisplay::set_XOR_mode);
    connect(set_replace_mode_action, &QAction::triggered,
            field_display, &FieldDisplay::set_replace_mode);

    connect(timer, &QTimer::timeout,
            this, &MainWindow::next_step);
}

void MainWindow::new_field() {}

void MainWindow::open_field() {
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"),
//                                                     QDir::currentPath(),
                                                     "../FIT_14202_Grachev_Life_Data",
                                                     "GameOfLife(*.txt);;All files(*.*)");
    if (!file_name.isEmpty()) {
        if (is_running) {
            stop_timer();
        }
        load_field_from_file(file_name);
    }
}

void MainWindow::save_field() {
    QString file_name = QFileDialog::getSaveFileName(this, tr("Save File"),
//                                                    QDir::currentPath(),
                                                     "../FIT_14202_Grachev_Life_Data",
                                                     "GameOfLife(*.txt);;All files(*.*)");
    if (!file_name.isEmpty()) {
        bool was_running = is_running;
        if (is_running) {
            stop_timer();
        }
        save_field_to_file(file_name);
        if (was_running) {
            start_timer();
        }
    }
}

void MainWindow::exit() {}

void MainWindow::next_step() {
    game_engine->next_step();
}

void MainWindow::stop_timer() {
    run_action->setChecked(false);
    clear_field_action->setEnabled(true);
    next_step_action->setEnabled(true);
    timer->stop();
    is_running = false;
}

void MainWindow::start_timer() {
    is_running = true;
    run_action->setChecked(true);
    clear_field_action->setEnabled(false);
    next_step_action->setEnabled(false);
    timer->start();
}

void MainWindow::toggle_run() {
    if (!is_running) {
        start_timer();
    } else {
        stop_timer();
    }
}

void MainWindow::clear_field() {
    game_engine->clear();
}

bool MainWindow::load_field_from_file(const QString & file_name) {
    QFile file(file_name);
    if (file.open(QIODevice::ReadOnly)) {
        bool ok = false;
        QTextStream input(&file);

        //  Read cols and rows count.
        QString line = input.readLine();
        if (line.isNull()) {
            return false;
        }
        QStringList line_parts = line.split(' ');
        if(line_parts.size() < 2) {
            return false;
        }
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
        if(line_parts.size() < 1) {
            return false;
        }
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
        if(line_parts.size() < 1) {
            return false;
        }
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
        if(line_parts.size() < 1) {
            return false;
        }
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
            if(line_parts.size() < 2) {
                return false;
            }
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

void MainWindow::save_field_to_file(const QString & file_name) {
    QFile file(file_name);

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream output(&file);
        const LifeStateField & state_field = game_engine->get_state_field();
        output << state_field.cols() << ' ' << state_field.rows() << endl;
        output << 1 << endl;
        output << field_display->get_cell_edge_size() << endl;
        uint64_t alive_cells_count = state_field.alive_cells_count();
        output << alive_cells_count << endl;
        for (uint32_t row = 0; row < state_field.rows(); ++row) {
            for (uint32_t col = 0; col < state_field.cols(); ++col) {
                if (state_field[row][col] == LifeStateField::ALIVE) {
                    output << col << ' ' << row << endl;
                }
            }
        }
    } else {
        // TODO: show error
    }
}