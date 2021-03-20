#pragma once
#include <filesystem>
#include <vector>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>

class CSVReader
{
public:
	CSVReader(const std::filesystem::path& name);
	~CSVReader();
	QStringList ReadRow();
	std::vector<long long> ReadLongLongRow();
	std::vector<int> ReadIntRow();
	bool AtEnd();
	void DiscardRow();
private:
	QFile* file;
	QTextStream* ts;
};

class CSVWriter
{
public:
	CSVWriter(const std::filesystem::path& path, const QString& filename);
	~CSVWriter();
	void WriteLine(const QString& str);
	void WriteLine(const QStringList& strList);
private:
	QFile* file;
	QTextStream* ts;
};