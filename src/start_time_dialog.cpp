/*
	SPDX-FileCopyrightText: 2026 Filipe Miranda

	SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "start_time_dialog.h"

#include "date_editor.h"
#include "time_editor.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>

//-----------------------------------------------------------------------------

StartTimeDialog::StartTimeDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle(tr("Change Start Time"));

	m_date = new DateEditor(this);
	m_time = new TimeEditor(this);

	QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
	connect(buttons, &QDialogButtonBox::accepted, this, &StartTimeDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, this, &StartTimeDialog::reject);

	QFormLayout* item_layout = new QFormLayout;
	item_layout->setContentsMargins(0, 0, 0, 0);
	item_layout->addRow(tr("Date:"), m_date);
	item_layout->addRow(tr("Start:"), m_time);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addLayout(item_layout);
	layout->addStretch();
	layout->addWidget(buttons);
}

//-----------------------------------------------------------------------------

QDateTime StartTimeDialog::startTime() const
{
	return QDateTime(m_date->date(), m_time->time());
}

//-----------------------------------------------------------------------------

void StartTimeDialog::setStartTime(const QDateTime& start)
{
	m_date->setDate(start.date());
	m_time->setTime(start.time());
}

//-----------------------------------------------------------------------------

#include "moc_start_time_dialog.cpp"
