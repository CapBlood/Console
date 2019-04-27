#include "lib.moc"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	this->setWindowTitle("Console");
	this->setMinimumSize(QSize(752, 512));

	QVBoxLayout *layout = new QVBoxLayout;
	Console *console = new Console;
	QWidget *window = new QWidget;

	layout->addWidget(console);
	window->setLayout(layout);
	setCentralWidget(window);
}

MainWindow::~MainWindow()
{

}

Console::Console(QWidget *parent) : QPlainTextEdit(parent)
{
	prompt = "->";
	QPalette p = palette();
	p.setColor(QPalette::Base, Qt::black);
	p.setColor(QPalette::Text, Qt::green);
	setPalette(p);
	insertPrompt(false);

	connect(this, &Console::onCommand, [] () {printf("sfgfdgdfg\n");});
}

void Console::insertPrompt(bool insertNewBlock)
{
	QTextCharFormat format;
    	format.setForeground(Qt::green);
    	textCursor().setBlockCharFormat(format);
    	if(insertNewBlock)
        	textCursor().insertBlock();
    	textCursor().insertText(prompt);
	scrollDown();
}

void Console::keyPressEvent(QKeyEvent *event)
{
	if(isLocked)
		return;


	if(event->key() >= 0x20 && event->key() <= 0x7e
       	&& (event->modifiers() == Qt::NoModifier || event->modifiers() == Qt::ShiftModifier))
        	QPlainTextEdit::keyPressEvent(event);
	else if(event->key() == Qt::Key_Backspace
       	&& event->modifiers() == Qt::NoModifier
       	&& textCursor().positionInBlock() > prompt.length())
        	QPlainTextEdit::keyPressEvent(event);
	else if((event->key() == Qt::Key_Left || event->key() == Qt::Key_Right)
	&& textCursor().positionInBlock() > prompt.length())
		QPlainTextEdit::keyPressEvent(event);
	else if(event->key() == Qt::Key_Return && event->modifiers() == Qt::NoModifier)
        	onEnter();
}

void Console::output(QString s)
{
	QTextCharFormat format;
    	format.setForeground(Qt::white);
    	textCursor().setBlockCharFormat(format);
    	textCursor().insertBlock();
    	textCursor().insertText(s);
    	insertPrompt(false);
    	isLocked = false;
}

void Console::onEnter()
{
	if(textCursor().positionInBlock() == prompt.length()) {
		insertPrompt();
        	return;
    	}

    	QString cmd = textCursor().block().text().mid(prompt.length());
    	emit onCommand(cmd);
    	isLocked = true;
}

void Console::scrollDown()
{
    QScrollBar *vbar = verticalScrollBar();
    vbar->setValue(vbar->maximum());
}

void Console::mousePressEvent(QMouseEvent *)
{
	setFocus();
}

void Console::mouseDoubleClickEvent(QMouseEvent *){}
void Console::contextMenuEvent(QContextMenuEvent *) {}
