#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QClipboard>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QKeyEvent>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QToolTip>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , _em(this)
  , _tsr(this)
  , _about(this)
{
  // 国际化
  QLocale defaultLocale;
  _tsr.load(defaultLocale, "", "", ":/i18n");
  qApp->installTranslator(&_tsr);

  // Designer UI
  ui->setupUi(this);

  // 语言菜单
  QActionGroup* langGroup = new QActionGroup(ui->menuLanguage);
  langGroup->setExclusive(true); // 如果使用 lambda 表达式，这里就不好做
  // 自动扫描 :/i18n 下的翻译文件创建菜单项
  QStringList filenames = QDir(":/i18n").entryList();
  for (auto filename : filenames) {
    filename.truncate(filename.lastIndexOf('.')); // 去掉 .qm 后缀

    auto locale = QLocale(filename);

    QAction* action = new QAction(locale.nativeLanguageName(), langGroup);
    action->setCheckable(true);
    action->setData(filename);
    ui->menuLanguage->addAction(action);

    if (locale.script() == defaultLocale.script())
      action->setChecked(true);
  }
  connect(langGroup,
          &QActionGroup::triggered,
          this,
          &MainWindow::when_langGroup_triggered);

  // 创建自定义组件
  ui->mainDialogLayout->addWidget(&_em);
  connect(
    &_em, &EnigmaMachine::charEncoded, this, &MainWindow::when_em_charEncoded);
  connect(
    &_em, &EnigmaMachine::backspace, this, &MainWindow::when_em_backspace);

  // 菜单栏
  connect(ui->actionWhatsEnigma, &QAction::triggered, []() {
    QDesktopServices::openUrl(
      tr("https://en.wikipedia.org/wiki/Enigma_machine"));
  });
  connect(ui->actionAbout, &QAction::triggered, [this]() { _about.show(); });
  connect(
    ui->actionTutorial, &QAction::triggered, [this]() { _tutorial.show(); });
  connect(ui->actionReset,
          &QAction::triggered,
          this,
          &MainWindow::when_actionReset_triggered);
}

MainWindow::~MainWindow()
{
  delete ui;
  qApp->removeTranslator(&_tsr);
}

void
MainWindow::retranslate_ui()
{
  ui->retranslateUi(this);
  _em.retranslate_ui();
}

void
MainWindow::keyPressEvent(QKeyEvent*)
{
  _em.set_focus();
}

void
MainWindow::changeEvent(QEvent* e)
{
  if (e) {
    switch (e->type()) {
      // this event is send if a translator is loaded
      case QEvent::LanguageChange: {
        retranslate_ui();
      } break;

      // this event is send, if the system, language changes
      case QEvent::LocaleChange: {
        auto locale = QLocale::system();
        if (!_tsr.load(locale, "", "", ":/i18n"))
          ui->statusbar->showMessage(
            tr("failed to load language file %1").arg(locale.language()));
      } break;

      default:
        break;
    }
  }
  QMainWindow::changeEvent(e);
}

void
MainWindow::when_em_charEncoded(QChar plain, QChar cipher)
{
  ui->inputs->setText(ui->inputs->text() + plain);
  ui->outputs->setText(ui->outputs->text() + cipher);
}

void
MainWindow::when_em_backspace()
{
  auto s = ui->inputs->text();
  if (s.isEmpty())
    return;

  s.remove(s.size() - 1, 1);
  ui->inputs->setText(s);
  s = ui->outputs->text();
  s.remove(s.size() - 1, 1);
  ui->outputs->setText(s);
}

void
MainWindow::when_langGroup_triggered(QAction* action)
{
  auto filename = action->data().toString();
  if (!_tsr.load(":/i18n/" + filename + ".qm")) {
    QMessageBox::critical(
      this, "Error", tr("cannot find language file for %1").arg(filename));
    return;
  }
  ui->statusbar->showMessage(tr("change language to %1").arg(filename));
}

void
MainWindow::when_actionReset_triggered()
{
  for (int i = ui->inputs->text().size(); --i != -1;) {
    _em.step_back();
  }
  ui->inputs->clear();
  ui->outputs->clear();
}

void
MainWindow::on_copyInButton_clicked()
{
  auto clipboard = QApplication::clipboard();
  clipboard->setText(ui->copyInButton->text());
  QToolTip::showText(
    ui->copyInButton->mapToGlobal(ui->copyInButton->rect().bottomLeft()),
    tr("outputs copied to clipboard"));
}

void
MainWindow::on_copyOutButton_clicked()
{
  auto clipboard = QApplication::clipboard();
  clipboard->setText(ui->copyOutButton->text());
  QToolTip::showText(
    ui->copyOutButton->mapToGlobal(ui->copyOutButton->rect().bottomLeft()),
    tr("outputs copied to clipboard"));
}
