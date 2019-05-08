#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPlainTextEdit>
#include <QTextCursor>
#include <QKeyEvent>
#include <QTextCharFormat>
#include <QScrollBar>
#include <QPalette>
#include <QTextBlock>
#include <QTextStream>

#include <stdio.h>
#include "console.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

};

class Console : public QPlainTextEdit
{
	Q_OBJECT

public:
	Console(QWidget *parent = nullptr);
	~Console();

public slots:
	void keyPressEvent(QKeyEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseDoubleClickEvent(QMouseEvent *);
	void contextMenuEvent(QContextMenuEvent *);
	void commandProccess(QString);
	void readOut();

signals:
	void onCommand(QString);

private:
	void insertPrompt(bool insertNewBlock = true);
	void onEnter();
	void output(QString s);
	void scrollDown();

	QTextStream *stream;
	QString prompt;
	bool isLocked;
};

#endif // MAINWINDOW_H
