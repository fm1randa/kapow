/*
	SPDX-FileCopyrightText: 2026 Filipe Miranda

	SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "start_time_editor.h"

#include "date_editor.h"
#include "time_editor.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWindow>

//-----------------------------------------------------------------------------

StartTimeEditor::StartTimeEditor(QWidget* parent)
	: QWidget(parent)
{
	m_date = new DateEditor(this);
	m_time = new TimeEditor(this);

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(m_date);
	layout->addWidget(m_time);
	setFocusProxy(m_time);

	connect(qApp, &QApplication::focusChanged, this, &StartTimeEditor::focusChanged);
}

//-----------------------------------------------------------------------------

QDateTime StartTimeEditor::startTime() const
{
	return QDateTime(m_date->date(), m_time->time());
}

//-----------------------------------------------------------------------------

void StartTimeEditor::setStartTime(const QDateTime& start)
{
	m_date->setDate(start.date());
	m_time->setTime(start.time());
}

//-----------------------------------------------------------------------------

bool StartTimeEditor::eventFilter(QObject* watched, QEvent* event)
{
	if (((watched == m_date) || (watched == m_time)) && (event->type() == QEvent::KeyPress)) {
		switch (static_cast<QKeyEvent*>(event)->key()) {
		case Qt::Key_Return:
		case Qt::Key_Enter:
			Q_EMIT accepted();
			return true;
		case Qt::Key_Escape:
			Q_EMIT cancelled();
			return true;
		default:
			break;
		}
	}

	// Switching to another application cancels; the application's own popups
	// and message boxes keep it active
	if ((watched == qApp) && (event->type() == QEvent::ApplicationStateChange)
			&& (static_cast<QApplicationStateChangeEvent*>(event)->applicationState() != Qt::ApplicationActive)) {
		Q_EMIT cancelled();
	}

	// Clicks reach the window before any widget, so each one is seen once here;
	// this also catches clicks on widgets that take no focus
	if ((event->type() == QEvent::MouseButtonPress) && isVisible() && watched->isWindowType()
			&& (static_cast<QWindow*>(watched) == window()->windowHandle())) {
		const QPointF pos = static_cast<QMouseEvent*>(event)->globalPosition();
		const QWidget* target = window()->childAt(window()->mapFromGlobal(pos.toPoint()));
		if ((target != this) && !isAncestorOf(target)) {
			Q_EMIT cancelled();
		}
	}

	return QWidget::eventFilter(watched, event);
}

//-----------------------------------------------------------------------------

void StartTimeEditor::hideEvent(QHideEvent* event)
{
	qApp->removeEventFilter(this);
	QWidget::hideEvent(event);
}

//-----------------------------------------------------------------------------

void StartTimeEditor::showEvent(QShowEvent* event)
{
	// Watch keys, clicks and application switches only while editing
	qApp->installEventFilter(this);
	QWidget::showEvent(event);
}

//-----------------------------------------------------------------------------

void StartTimeEditor::focusChanged(QWidget* old, QWidget* now)
{
	Q_UNUSED(old)

	if (!isVisible()) {
		return;
	}

	// Focus moving to another window (the calendar popup, a warning, another
	// application) or between the date and time editors keeps the edit open
	if (!now || (now->window() != window()) || isAncestorOf(now)) {
		return;
	}

	Q_EMIT cancelled();
}

//-----------------------------------------------------------------------------

#include "moc_start_time_editor.cpp"
