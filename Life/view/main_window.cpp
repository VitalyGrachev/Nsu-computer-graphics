#include "main_window.h"

#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QScopedPointer>

MainWindow::MainWindow(QWidget * parent)
        : QMainWindow(parent),
          field_scroll_area(new QScrollArea(this)),
          timer(new QTimer(this)),
          is_running(false) {

    create_actions();
    create_menus();
    create_toolbar();

    timer->setInterval(timer_interval_msec);

    setCentralWidget(field_scroll_area);

    field_scroll_area->setVisible(true);
    field_scroll_area->setWidgetResizable(true);

    create_new_field(default_cols, default_rows, default_edge_size);

    field_scroll_area->setPalette(field_display->palette());

    connect_all();

    setWindowTitle(tr("FIT_14202_Grachev_Life"));
    resize(800, 600);
}

void MainWindow::create_actions() {
    new_field_action = new QAction(tr("&New"), this);
    new_field_action->setToolTip(tr("Create new field"));

    open_field_action = new QAction(tr("&Open"), this);
    open_field_action->setToolTip(tr("Load field from file"));

    save_field_action = new QAction(tr("&Save"), this);
    save_field_action->setToolTip(tr("Save field to file"));

    next_step_action = new QAction(tr("Next"), this);
    next_step_action->setToolTip(tr("Next generation"));

    run_action = new QAction(tr("Run"), this);
    run_action->setToolTip(tr("Run continuous generation change"));
    run_action->setCheckable(true);

    clear_field_action = new QAction(tr("Clear"), this);
    clear_field_action->setToolTip(tr("Clear field"));

    set_options_action = new QAction(tr("Options"), this);
    set_options_action->setToolTip(tr("Show options dialog"));

    toggle_impacts_action = new QAction(tr("Impact"), this);
    toggle_impacts_action->setToolTip(tr("Show impacts"));
    toggle_impacts_action->setCheckable(true);

    set_replace_mode_action = new QAction(tr("Replace"), this);
    set_replace_mode_action->setToolTip(tr("Replace mode"));
    set_replace_mode_action->setCheckable(true);

    set_xor_mode_action = new QAction(tr("XOR"), this);
    set_xor_mode_action->setToolTip(tr("XOR mode"));
    set_xor_mode_action->setCheckable(true);

    mode_action_group = new QActionGroup(this);
    mode_action_group->addAction(set_replace_mode_action);
    mode_action_group->addAction(set_xor_mode_action);
    set_xor_mode_action->setChecked(true);

    show_about_action = new QAction(tr("About"), this);
}

void MainWindow::create_menus() {
    file_menu = new QMenu(tr("&File"), this);
    file_menu->addAction(new_field_action);
    file_menu->addAction(open_field_action);
    file_menu->addAction(save_field_action);

    QMenu * mode_menu = new QMenu(tr("Mode"), this);
    mode_menu->addAction(set_replace_mode_action);
    mode_menu->addAction(set_xor_mode_action);

    edit_menu = new QMenu(tr("&Edit"), this);
    edit_menu->addAction(clear_field_action);
    edit_menu->addMenu(mode_menu);
    edit_menu->addAction(set_options_action);

    view_menu = new QMenu(tr("&View"), this);
    view_menu->addAction(toggle_impacts_action);

    action_menu = new QMenu(tr("&Action"), this);
    action_menu->addAction(next_step_action);
    action_menu->addAction(run_action);

    help_menu = new QMenu(tr("Help"), this);
    help_menu->addAction(show_about_action);

    menuBar()->addMenu(file_menu);
    menuBar()->addMenu(edit_menu);
    menuBar()->addMenu(view_menu);
    menuBar()->addMenu(action_menu);
    menuBar()->addMenu(help_menu);
}

void MainWindow::create_toolbar() {
    toolbar = new QToolBar(this);
    toolbar->addAction(new_field_action);
    toolbar->addAction(open_field_action);
    toolbar->addAction(save_field_action);
    toolbar->addSeparator();
    toolbar->addAction(toggle_impacts_action);
    toolbar->addSeparator();
    toolbar->addAction(clear_field_action);
    toolbar->addAction(set_options_action);
    toolbar->addSeparator();
    toolbar->addAction(set_replace_mode_action);
    toolbar->addAction(set_xor_mode_action);
    toolbar->addSeparator();
    toolbar->addAction(next_step_action);
    toolbar->addAction(run_action);
    toolbar->addSeparator();
    toolbar->addAction(show_about_action);

    toolbar->setMovable(false);

    addToolBar(toolbar);
}

RulesGroupBox * MainWindow::create_rules_group_box() {
    LifeGameEngine & lge = *game_engine;
    RulesGroupBox * rules_group_box = new RulesGroupBox(lge.get_live_begin(), lge.get_live_end(),
                                                        lge.get_birth_begin(), lge.get_birth_end(),
                                                        lge.get_near_neighbour_impact(),
                                                        lge.get_far_neighbour_impact(), this);
    return rules_group_box;
}

CellSizeGroupBox * MainWindow::create_cell_size_group_box() {
    CellSizeGroupBox * cell_size_group_box = new CellSizeGroupBox(field_display->get_cell_edge_size(),
                                                                  min_edge_size, max_edge_size, this);
    return cell_size_group_box;
}

FieldSizeGroupBox * MainWindow::create_field_size_group_box() {
    FieldSizeGroupBox * field_size_group_box = new FieldSizeGroupBox(game_engine->cols(),
                                                                     game_engine->rows(),
                                                                     min_cols, max_cols,
                                                                     min_rows, max_rows, this);
    return field_size_group_box;
}

QDialog * MainWindow::create_new_field_dialog() {
    NewFieldDialog * new_field_dialog = new NewFieldDialog(create_field_size_group_box(),
                                                           create_cell_size_group_box());
    connect(new_field_dialog, &NewFieldDialog::create_new_field,
            this, &MainWindow::create_new_field);
    return new_field_dialog;
}

QDialog * MainWindow::create_options_dialog() {
    OptionsDialog * options_dialog = new OptionsDialog(create_field_size_group_box(),
                                                       create_cell_size_group_box(),
                                                       create_rules_group_box());
    connect(options_dialog, &OptionsDialog::set_options,
            this, &MainWindow::set_options);
    return options_dialog;
}

void MainWindow::connect_field_display() {
    connect(field_display, &FieldDisplay::set_cell,
            this, &MainWindow::set_cell);

    connect(toggle_impacts_action, &QAction::triggered,
            field_display, &FieldDisplay::set_impacts);

    connect(set_xor_mode_action, &QAction::triggered,
            field_display, &FieldDisplay::set_XOR_mode);

    connect(set_replace_mode_action, &QAction::triggered,
            field_display, &FieldDisplay::set_replace_mode);
}

void MainWindow::connect_all() {
    connect(new_field_action, &QAction::triggered,
            this, &MainWindow::new_field);
    connect(open_field_action, &QAction::triggered,
            this, &MainWindow::open_field);
    connect(save_field_action, &QAction::triggered,
            this, &MainWindow::save_field);

    connect(next_step_action, &QAction::triggered,
            this, &MainWindow::next_step);
    connect(run_action, &QAction::triggered,
            this, &MainWindow::toggle_run);
    connect(clear_field_action, &QAction::triggered,
            this, &MainWindow::clear_field);
    connect(set_options_action, &QAction::triggered,
            this, &MainWindow::show_options);

    connect(show_about_action, &QAction::triggered,
            this, &MainWindow::show_about);

    connect(timer, &QTimer::timeout,
            this, &MainWindow::next_step);
}

void MainWindow::new_field() {
    QScopedPointer<QDialog> new_field_dialog(create_new_field_dialog());
    new_field_dialog->exec();
}

void MainWindow::show_options() {
    QScopedPointer<QDialog> options_dialog(create_options_dialog());
    options_dialog->exec();
}

void MainWindow::open_field() {
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"),
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
    bool was_running = is_running;
    if (is_running) {
        stop_timer();
    }
    QString file_name = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                     "../FIT_14202_Grachev_Life_Data",
                                                     "GameOfLife(*.txt);;All files(*.*)");
    if (!file_name.isEmpty()) {
        save_field_to_file(file_name);
    }
    if (was_running) {
        start_timer();
    }
}

void MainWindow::set_cell(uint32_t col, uint32_t row, CellState state) {
    if (!is_running) {
        game_engine->set_cell(col, row, state);
    }
}

void MainWindow::next_step() {
    game_engine->next_step();
}

void MainWindow::start_timer() {
    is_running = true;
    run_action->setChecked(true);
    clear_field_action->setEnabled(false);
    next_step_action->setEnabled(false);
    timer->start();
}

void MainWindow::stop_timer() {
    run_action->setChecked(false);
    clear_field_action->setEnabled(true);
    next_step_action->setEnabled(true);
    timer->stop();
    is_running = false;
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

void MainWindow::show_about() {
    QMessageBox::about(this, tr("About FIT_14202_Grachev_Life"),
                       tr("Life Version 1.0, NSU FIT 14202 Grachev"));
}

void MainWindow::set_options(int cols, int rows,
                             int cell_edge,
                             double live_begin, double live_end,
                             double birth_begin, double birth_end,
                             double first_impact, double second_impact) {
    if (cols != game_engine->cols() || rows != game_engine->rows()) {
        std::unique_ptr<SignalNotifier> notifier(new SignalNotifier());
        std::unique_ptr<LifeGameEngine> engine(new LifeGameEngine(cols, rows, notifier.get()));
        std::unique_ptr<FieldDisplay> display(new FieldDisplay(engine.get(), cell_edge));
        connect(notifier.get(), SIGNAL(notification()),
                display.get(), SLOT(model_changed()));

        uint32_t min_cols = std::min(static_cast<uint32_t>(cols), game_engine->cols());
        uint32_t min_rows = std::min(static_cast<uint32_t>(rows), game_engine->rows());

        const LifeStateField & states = game_engine->get_state_field();
        for (uint32_t r = 0; r < min_rows; ++r) {
            for (uint32_t c = 0; c < min_cols; ++c) {
                if (states[r][c] == LifeStateField::ALIVE) {
                    engine->set_cell(c, r);
                }
            }
        }
        set_model_and_view(std::move(notifier), std::move(engine), std::move(display));
    } else if (cell_edge != field_display->get_cell_edge_size()) {
        std::unique_ptr<FieldDisplay> display(new FieldDisplay(game_engine.get(), cell_edge));
        connect(signal_notifier.get(), SIGNAL(notification()),
                display.get(), SLOT(model_changed()));
        set_model_and_view(std::move(signal_notifier), std::move(game_engine), std::move(display));
    }

    game_engine->set_neighbour_impacts(first_impact, second_impact);
    game_engine->set_cell_fate_conditions(birth_begin, birth_end, live_begin, live_end);
}

void MainWindow::set_model_and_view(std::unique_ptr<SignalNotifier> && notifier,
                                    std::unique_ptr<LifeGameEngine> && engine,
                                    std::unique_ptr<FieldDisplay> && display) {
    signal_notifier = std::move(notifier);
    game_engine = std::move(engine);
    field_display = display.release();
    field_scroll_area->setWidget(field_display);

    toggle_impacts_action->setChecked(false);
    toggle_impacts_action->setEnabled(field_display->get_cell_edge_size() > HexGridCanvas::font_size);

    connect_field_display();
}

void MainWindow::create_new_field(int cols, int rows, int cell_edge) {
    try {
        std::unique_ptr<SignalNotifier> notifier(new SignalNotifier());
        std::unique_ptr<LifeGameEngine> engine(new LifeGameEngine(cols, rows, notifier.get()));
        std::unique_ptr<FieldDisplay> display(new FieldDisplay(engine.get(), cell_edge));
        connect(notifier.get(), SIGNAL(notification()),
                display.get(), SLOT(model_changed()));
        set_model_and_view(std::move(notifier), std::move(engine), std::move(display));
    } catch (std::bad_alloc & e) {
        QMessageBox::warning(this, tr("Not enough memory"),
                             tr("Not enough memory to fit this field."));
    }
}

bool MainWindow::load_field_from_file(const QString & file_name) {
    QFile file(file_name);
    try {
        if (file.open(QIODevice::ReadOnly)) {
            bool ok = false;
            QTextStream input(&file);

            //  Read cols and rows count.
            QString line = input.readLine();
            if (line.isNull()) {
                throw std::runtime_error("Unexpected end of file.");
            }
            QStringList line_parts = line.split(' ');
            if (line_parts.size() < 2) {
                throw std::runtime_error("Rows and/or columns count not stated.");
            }
            uint32_t cols = line_parts[0].toUInt(&ok);
            if (!ok) {
                throw std::runtime_error("Expected columns count, found not a number.");
            }
            uint32_t rows = line_parts[1].toUInt(&ok);
            if (!ok) {
                throw std::runtime_error("Expected row count, found not a number.");
            }
            if (line_parts.size() > 2 && !line_parts[2].startsWith("//")) {
                throw std::runtime_error("Found garbage after row and column numbers.");
            }

            // Read cell separator width.
            line = input.readLine();
            if (line.isNull()) {
                throw std::runtime_error("Unexpected end of file.");
            }
            line_parts = line.split(' ');
            if (line_parts.size() < 1) {
                throw std::runtime_error("Cell separator width not stated.");
            }
            uint32_t cell_separator_width = line_parts[0].toUInt(&ok);
            if (!ok) {
                throw std::runtime_error("Expected cell separator width, found not a number.");
            }
            if (line_parts.size() > 1 && !line_parts[1].startsWith("//")) {
                throw std::runtime_error("Found garbage after cell separator width.");
            }

            // Read cell edge size.
            line = input.readLine();
            if (line.isNull()) {
                throw std::runtime_error("Unexpected end of file.");
            }
            line_parts = line.split(' ');
            if (line_parts.size() < 1) {
                throw std::runtime_error("Cell edge size not stated.");
            }
            uint32_t cell_edge_size = line_parts[0].toUInt(&ok);
            if (!ok) {
                throw std::runtime_error("Expected cell edge size, found not a number.");
            }
            if (line_parts.size() > 1 && !line_parts[1].startsWith("//")) {
                throw std::runtime_error("Found garbage after cell edge size.");
            }

            // Read alive cell count.
            line = input.readLine();
            if (line.isNull()) {
                throw std::runtime_error("Unexpected end of file.");
            }
            line_parts = line.split(' ');
            if (line_parts.size() < 1) {
                throw std::runtime_error("Alive cell count not stated.");
            }
            uint32_t alive_cell_count = line_parts[0].toUInt(&ok);
            if (!ok) {
                throw std::runtime_error("Expected alive cell count, found not a number.");
            }
            if (line_parts.size() > 1 && !line_parts[1].startsWith("//")) {
                throw std::runtime_error("Found garbage after alive cell count.");
            }

            std::unique_ptr<SignalNotifier> notifier(new SignalNotifier());
            std::unique_ptr<LifeGameEngine> engine(new LifeGameEngine(cols, rows, notifier.get()));
            std::unique_ptr<FieldDisplay> display(new FieldDisplay(engine.get(), cell_edge_size));
            connect(notifier.get(), SIGNAL(notification()),
                    display.get(), SLOT(model_changed()));

            // Read alive cell positions.
            for (uint32_t i = 0; i < alive_cell_count; ++i) {
                line = input.readLine();
                if (line.isNull()) {
                    throw std::runtime_error("Unexpected end of file.");
                }
                line_parts = line.split(' ');
                if (line_parts.size() < 2) {
                    throw std::runtime_error("Alive cell positions not stated.");
                }
                uint32_t row = line_parts[1].toUInt(&ok);
                if (!ok) {
                    throw std::runtime_error("Expected alive cell row, found not a number.");
                }
                if (row >= rows) {
                    throw std::runtime_error("Alive cell row greater, than row count.");
                }
                if (line_parts.size() > 2 && !line_parts[2].startsWith("//")) {
                    throw std::runtime_error("Found garbage after alive cell row and column numbers.");
                }
                uint32_t col = line_parts[0].toUInt(&ok);
                if (!ok) {
                    throw std::runtime_error("Expected alive cell column, found not a number.");
                }
                if (col >= cols - (row % 2)) {
                    throw std::runtime_error("Alive cell column greater, than column count.");
                }

                engine->set_cell(col, row);
            }

            if (!input.atEnd()) {
                throw std::runtime_error("Found garbage at end of file.");
            }

            set_model_and_view(std::move(notifier), std::move(engine), std::move(display));
        }
    }
    catch (std::runtime_error & e) {
        QMessageBox::warning(this, tr("Error in file"),
                             tr("File cannot be read due to error: ") + tr(e.what()));
        return false;
    }
    catch (std::bad_alloc & e) {
        QMessageBox::warning(this, tr("Not enough memory"),
                             tr("File cannot be read: not enough memory to fit this field."));
        return false;
    }
    return true;
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
        QMessageBox::warning(this, tr("Cannot save file."),
                             tr("File cannot be saved: file cannot be opened."));
    }
}