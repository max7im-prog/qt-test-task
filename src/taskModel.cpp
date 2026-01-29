
#include "taskModel.hpp"

TaskModel::TaskModel(QObject *parent) : QAbstractListModel(parent) {}

int TaskModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;
  return m_tasks.size();
}

QVariant TaskModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() < 0 || index.row() >= m_tasks.size())
    return {};

  const Task &task = m_tasks.at(index.row());

  switch (role) {
  case NameRole:
    return task.name;
  case ProgressRole:
    return task.progress;
  case StatusRole:
    return task.status;
  default:
    return {};
  }
}

QHash<int, QByteArray> TaskModel::roleNames() const {
  return {
      {NameRole, "name"}, {ProgressRole, "progress"}, {StatusRole, "status"}};
}

void TaskModel::removeTask(const QString &name) {
  int index = findTaskIndex(name);
  if (index == -1)
    return;

  beginRemoveRows(QModelIndex(), index, index);
  m_tasks.removeAt(index);
  endRemoveRows();
}

void TaskModel::addTask(const QString &name) {
  if (findTaskIndex(name) != -1)
    return;

  beginInsertRows(QModelIndex(), m_tasks.size(), m_tasks.size());
  m_tasks.push_back({name, 0, "Running"});
  endInsertRows();
}

void TaskModel::updateProgress(const QString &name, int progress,
                               const QString &status) {
  int index = findTaskIndex(name);
  if (index == -1)
    return;

  Task &task = m_tasks[index];
  task.progress = progress;
  if (!status.isEmpty())
    task.status = status;

  QModelIndex modelIndex = createIndex(index, 0);
  emit dataChanged(modelIndex, modelIndex, {ProgressRole, StatusRole});
}

void TaskModel::finishTask(const QString &name) {
  int index = findTaskIndex(name);
  if (index == -1)
    return;

  m_tasks[index].progress = 100;
  m_tasks[index].status = "Finished";

  QModelIndex modelIndex = createIndex(index, 0);
  emit dataChanged(modelIndex, modelIndex, {ProgressRole, StatusRole});
}

void TaskModel::clear() {
  beginResetModel();
  m_tasks.clear();
  endResetModel();
}

int TaskModel::findTaskIndex(const QString &name) const {
  for (int i = 0; i < m_tasks.size(); ++i) {
    if (m_tasks[i].name == name)
      return i;
  }
  return -1;
}
