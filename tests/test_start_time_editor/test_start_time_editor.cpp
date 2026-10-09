/*
	SPDX-FileCopyrightText: 2026 Filipe Miranda

	SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "test_start_time_editor.h"

#include "start_time_editor.h"

#include <QApplication>
#include <QDateEdit>
#include <QLabel>
#include <QLineEdit>
#include <QSignalSpy>
#include <QTest>
#include <QTimeEdit>
#include <QVBoxLayout>
#include <QWindow>

//-----------------------------------------------------------------------------

namespace
{

// Tell the application it lost or regained activation, as when the user switches to another application
void setApplicationState(Qt::ApplicationState state)
{
	QApplicationStateChangeEvent event(state);
	QCoreApplication::sendEvent(qApp, &event);
}

// Click the center of a widget the way a user does: through its window
void clickThroughWindow(QWidget* widget)
{
	QWidget* window = widget->window();
	const QPoint pos = widget->mapTo(window, widget->rect().center());
	QTest::mouseClick(window->windowHandle(), Qt::LeftButton, Qt::NoModifier, pos);
}

}

//-----------------------------------------------------------------------------

void TestStartTimeEditor::enterConfirmsEditedStartTime()
{
	QWidget window;
	StartTimeEditor* editor = new StartTimeEditor(&window);
	editor->setStartTime(QDateTime(QDate(2026, 10, 8), QTime(9, 25, 0)));
	window.show();
	QVERIFY(QTest::qWaitForWindowExposed(&window));

	QSignalSpy accepted(editor, &StartTimeEditor::accepted);
	QSignalSpy cancelled(editor, &StartTimeEditor::cancelled);

	QTimeEdit* time = editor->findChild<QTimeEdit*>();
	QVERIFY(time);
	time->setTime(QTime(9, 10, 0));
	QTest::keyClick(time, Qt::Key_Return);

	QCOMPARE(accepted.count(), 1);
	QCOMPARE(cancelled.count(), 0);
	QCOMPARE(editor->startTime(), QDateTime(QDate(2026, 10, 8), QTime(9, 10, 0)));
}

//-----------------------------------------------------------------------------

void TestStartTimeEditor::escapeCancels()
{
	QWidget window;
	StartTimeEditor* editor = new StartTimeEditor(&window);
	window.show();
	QVERIFY(QTest::qWaitForWindowExposed(&window));

	QSignalSpy accepted(editor, &StartTimeEditor::accepted);
	QSignalSpy cancelled(editor, &StartTimeEditor::cancelled);

	QDateEdit* date = editor->findChild<QDateEdit*>();
	QVERIFY(date);
	QTest::keyClick(date, Qt::Key_Escape);

	QCOMPARE(accepted.count(), 0);
	QCOMPARE(cancelled.count(), 1);
}

//-----------------------------------------------------------------------------

void TestStartTimeEditor::focusLeavingEditorCancels()
{
	QWidget window;
	StartTimeEditor* editor = new StartTimeEditor(&window);
	QLineEdit* other = new QLineEdit(&window);
	QVBoxLayout* layout = new QVBoxLayout(&window);
	layout->addWidget(editor);
	layout->addWidget(other);
	window.show();
	window.activateWindow();
	QVERIFY(QTest::qWaitForWindowActive(&window));

	QTimeEdit* time = editor->findChild<QTimeEdit*>();
	time->setFocus();
	QTRY_VERIFY(time->hasFocus());

	QSignalSpy accepted(editor, &StartTimeEditor::accepted);
	QSignalSpy cancelled(editor, &StartTimeEditor::cancelled);

	other->setFocus();
	QTRY_VERIFY(other->hasFocus());

	QCOMPARE(accepted.count(), 0);
	QCOMPARE(cancelled.count(), 1);
}

//-----------------------------------------------------------------------------

void TestStartTimeEditor::focusMovingBetweenEditorsKeepsEditing()
{
	QWidget window;
	StartTimeEditor* editor = new StartTimeEditor(&window);
	QLineEdit* other = new QLineEdit(&window);
	QVBoxLayout* layout = new QVBoxLayout(&window);
	layout->addWidget(editor);
	layout->addWidget(other);
	window.show();
	window.activateWindow();
	QVERIFY(QTest::qWaitForWindowActive(&window));

	QDateEdit* date = editor->findChild<QDateEdit*>();
	QTimeEdit* time = editor->findChild<QTimeEdit*>();
	date->setFocus();
	QTRY_VERIFY(date->hasFocus());

	QSignalSpy cancelled(editor, &StartTimeEditor::cancelled);

	time->setFocus();
	QTRY_VERIFY(time->hasFocus());
	date->setFocus();
	QTRY_VERIFY(date->hasFocus());

	QCOMPARE(cancelled.count(), 0);
}

//-----------------------------------------------------------------------------

void TestStartTimeEditor::focusMovingToAnotherWindowKeepsEditing()
{
	// The calendar popup and the conflict warning are separate windows
	QWidget window;
	StartTimeEditor* editor = new StartTimeEditor(&window);
	QVBoxLayout* layout = new QVBoxLayout(&window);
	layout->addWidget(editor);
	window.show();
	window.activateWindow();
	QVERIFY(QTest::qWaitForWindowActive(&window));

	QTimeEdit* time = editor->findChild<QTimeEdit*>();
	time->setFocus();
	QTRY_VERIFY(time->hasFocus());

	QSignalSpy cancelled(editor, &StartTimeEditor::cancelled);

	QWidget popup(&window, Qt::Popup);
	QLineEdit* field = new QLineEdit(&popup);
	popup.show();
	QVERIFY(QTest::qWaitForWindowExposed(&popup));
	field->setFocus();
	QTRY_VERIFY(field->hasFocus());
	popup.close();
	window.activateWindow();
	QVERIFY(QTest::qWaitForWindowActive(&window));

	QCOMPARE(cancelled.count(), 0);
}

//-----------------------------------------------------------------------------

void TestStartTimeEditor::clickingElsewhereCancels()
{
	// Labels, and push buttons on macOS, take no focus when clicked
	QWidget window;
	StartTimeEditor* editor = new StartTimeEditor(&window);
	QLabel* label = new QLabel(QStringLiteral("00:00:00"), &window);
	QVBoxLayout* layout = new QVBoxLayout(&window);
	layout->addWidget(editor);
	layout->addWidget(label);
	window.show();
	QVERIFY(QTest::qWaitForWindowExposed(&window));

	QSignalSpy cancelled(editor, &StartTimeEditor::cancelled);

	clickThroughWindow(label);

	QCOMPARE(cancelled.count(), 1);
}

//-----------------------------------------------------------------------------

void TestStartTimeEditor::clickingEditorKeepsEditing()
{
	QWidget window;
	StartTimeEditor* editor = new StartTimeEditor(&window);
	QVBoxLayout* layout = new QVBoxLayout(&window);
	layout->addWidget(editor);
	window.show();
	QVERIFY(QTest::qWaitForWindowExposed(&window));

	QSignalSpy cancelled(editor, &StartTimeEditor::cancelled);

	clickThroughWindow(editor->findChild<QTimeEdit*>());
	clickThroughWindow(editor->findChild<QDateEdit*>());

	QCOMPARE(cancelled.count(), 0);
}

//-----------------------------------------------------------------------------

void TestStartTimeEditor::switchingApplicationCancels()
{
	QWidget window;
	StartTimeEditor* editor = new StartTimeEditor(&window);
	QVBoxLayout* layout = new QVBoxLayout(&window);
	layout->addWidget(editor);
	window.show();
	QVERIFY(QTest::qWaitForWindowExposed(&window));

	QSignalSpy accepted(editor, &StartTimeEditor::accepted);
	QSignalSpy cancelled(editor, &StartTimeEditor::cancelled);

	setApplicationState(Qt::ApplicationInactive);
	setApplicationState(Qt::ApplicationActive);

	QCOMPARE(accepted.count(), 0);
	QCOMPARE(cancelled.count(), 1);
}

//-----------------------------------------------------------------------------

void TestStartTimeEditor::hiddenEditorIgnoresSwitchingApplication()
{
	QWidget window;
	StartTimeEditor* editor = new StartTimeEditor(&window);
	QVBoxLayout* layout = new QVBoxLayout(&window);
	layout->addWidget(editor);
	window.show();
	QVERIFY(QTest::qWaitForWindowExposed(&window));
	editor->hide();

	QSignalSpy cancelled(editor, &StartTimeEditor::cancelled);

	setApplicationState(Qt::ApplicationInactive);
	setApplicationState(Qt::ApplicationActive);

	QCOMPARE(cancelled.count(), 0);
}

//-----------------------------------------------------------------------------

QTEST_MAIN(TestStartTimeEditor)

#include "moc_test_start_time_editor.cpp"
