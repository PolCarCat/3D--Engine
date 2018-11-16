#ifndef __IMPORTER_H__
#define __IMPORTER_H__


class Importer
{
public:
	Importer() {}
	~Importer() {}

	virtual bool Load()
	{
		return true;
	}

	virtual bool Save()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

};

#endif // !__IMPORTER_H__