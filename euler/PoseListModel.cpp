/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include "PoseListModel.h"
#include <QApplication>
#include <QPalette>
#include <QBrush>
#include <QDir>
#include <SkeletonSystem.h>
#include <PoseSpaceDeformer.h>
#include <SkeletonPose.h>

FileListModel::FileListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

//![4]
int FileListModel::rowCount(const QModelIndex & /* parent */) const
{
    return fileCount;
}

QVariant FileListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    
    if (index.row() >= fileList.size() || index.row() < 0)
        return QVariant();
    
    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return fileList.at(index.row());

    return QVariant();
}

Qt::ItemFlags FileListModel::flags(const QModelIndex &index) const
{
 if (!index.isValid())
	 return Qt::ItemIsEnabled;

 return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool FileListModel::setData(const QModelIndex &index,
                               const QVariant &value, int role)
{
 if (index.isValid() && role == Qt::EditRole) {

	QString preName = fileList.at(index.row());
	
	m_deformer->renamePose(preName.toUtf8().data(), value.toString().toUtf8().data());
	 fileList.replace(index.row(), value.toString());
	 emit dataChanged(index, index);
	 return true;
 }
 return false;
}
 
bool FileListModel::canFetchMore(const QModelIndex & /* index */) const
{
    if (fileCount < fileList.size())
        return true;
    else
        return false;
}
//![1]

//![2]
void FileListModel::fetchMore(const QModelIndex & /* index */)
{
    int remainder = fileList.size() - fileCount;
    int itemsToFetch = qMin(100, remainder);

    beginInsertRows(QModelIndex(), fileCount, fileCount+itemsToFetch-1);
    
    fileCount += itemsToFetch;

    endInsertRows();

    emit numberPopulated(itemsToFetch);
}
//![2]

//![0]
void FileListModel::setDeformer(PoseSpaceDeformer * deformer)
{
	m_deformer = deformer;
    shoPoses();
}

void FileListModel::shoPoses()
{
	fileList.clear();
	
	unsigned np = m_deformer->numPoses();
	for(unsigned i = 0; i < np; i++)
		fileList<<tr(m_deformer->pose(i)->name().c_str());
	
    fileCount = 0;
    reset();
}

void FileListModel::addPose()
{
	m_deformer->addPose();
	shoPoses();
}
//![0]

