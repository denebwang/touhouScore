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