/*
	SPDX-FileCopyrightText: 2026 Filipe Miranda

	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef KAPOW_START_TIME_EDITOR_H
#define KAPOW_START_TIME_EDITOR_H

class DateEditor;
class TimeEditor;

#include <QDateTime>
#include <QWidget>

class StartTimeEditor : public QWidget
{
	Q_OBJECT

public:
	explicit StartTimeEditor(QWidget* parent = nullptr);

	QDateTime startTime() const;
	void setStartTime(const QDateTime& start);

protected:
	bool eventFilter(QObject* watched, QEvent* event) override;

Q_SIGNALS:
	void accepted();
	void cancelled();

private Q_SLOTS:
	void focusChanged(QWidget* old, QWidget* now);

private:
	DateEditor* m_date;
	TimeEditor* m_time;
};

#endif // KAPOW_START_TIME_EDITOR_H
