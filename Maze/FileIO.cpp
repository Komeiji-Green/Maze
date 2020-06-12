/*
* cpp文件：FileIO.cpp
* 作用：存档系统的实现
*/

#include "ExFileIO.h"

	#pragma region mazesave:地图信息

void filename(wchar_t* _filename, int size, const Options& op)
{
	swprintf_s(_filename, size, L".\\mazesave\\%d_%d_%d.dat\0", op.id, op.mp, op.df);
}

void readFromMapData(ifstream& inf, vector<vector<unsigned char> >& map, MapParam& cfg)
{
	inf.read(reinterpret_cast<char*>(&cfg), sizeof(MapParam));

	map.resize(cfg.m);
	for (int i = 0; i < cfg.m; i++)
	{
		map[i].resize(cfg.n);
		inf.read(reinterpret_cast<char*>(map[i].data()), map[i].size());
	}
}

void writeToMapData(ofstream& ouf, const vector<vector<unsigned char> >& map, const MapParam& cfg)
{
	ouf.write(reinterpret_cast<const char*>(&cfg), sizeof(MapParam));

	for (int i = 0; i < map.size(); i++)
	{
		ouf.write(reinterpret_cast<const char*>(map[i].data()), map[i].size());
	}
}

#pragma endregion

	#pragma region playerdata:存档信息

// 负责返回新角色id
int newid()
{
	ofstream output("playerdata.dat", ios::binary | ios::app);
	if (output)
	{
		const int TUPLE_SIZE = sizeof(Tuple_Playerinfo);

		output.seekp(0, ios::end);
		size_t id = output.tellp() / TUPLE_SIZE;

		output.close();

		return id;
	}
	output.close();
	return -1;
}

// 内部接口
void readPlayerData(ifstream& input, vector<Tuple_Playerinfo>& data)
{
	const int TUPLE_SIZE = sizeof(Tuple_Playerinfo);
	if (input)
	{
		input.seekg(0, ios::end);
		data.resize(input.tellg() / TUPLE_SIZE);
		input.seekg(0, ios::beg);
		input.read(reinterpret_cast<char*>(data.data()), (streampos)data.size()* (streampos)TUPLE_SIZE);
	}
}

void writePlayerData(ofstream& output, const vector<Tuple_Playerinfo>& data)
{
	const int TUPLE_SIZE = sizeof(Tuple_Playerinfo);
	if (output)
	{
		output.seekp(0, ios::beg);
		output.write(reinterpret_cast<const char*>(data.data()), (streampos)data.size()* (streampos)TUPLE_SIZE);
	}
}

// 按player查找playerdata.dat, 若找到，返回id并将对应信息写入info
int searchPlayer(const wchar_t* player, Tuple_Playerinfo& info)
{
	vector<Tuple_Playerinfo> data;

	ifstream input("playerdata.dat", ios::binary);
	readPlayerData(input, data);
	input.close();

	Tuple_Playerinfo target(player);

	auto pfind = std::lower_bound(data.begin(), data.end(), target);
	if (pfind != data.end() && (*pfind == target))
	{
		info = *pfind;
		return (*pfind).id;
	}
	else
		return -1;
}

// 添加存档信息
void addPlayerData(const Options& op)
{
	vector<Tuple_Playerinfo> data;

	ifstream input("playerdata.dat", ios::binary);
	readPlayerData(input, data);
	input.close();

	Tuple_Playerinfo target(op);

	auto pfind = std::lower_bound(data.begin(), data.end(), target);
	if ((pfind != data.end()) && (*pfind == target))
	{
		pfind->mode = (pfind->mode | target.mode);
	}
	else
	{
		data.insert(pfind, 1, target);
	}

	ofstream output("playerdata.dat", ios::binary);
	writePlayerData(output, data);
	output.close();
}

// 删除存档信息
void deletePlayerData(const Options& op)
{
	vector<Tuple_Playerinfo> data;

	ifstream input("playerdata.dat", ios::binary);
	readPlayerData(input, data);
	input.close();

	Tuple_Playerinfo target(op);

	auto pfind = std::lower_bound(data.begin(), data.end(), target);
	if ((pfind != data.end()) && (*pfind == target))
	{
		pfind->mode = (pfind->mode & (~(target.mode)));
	}

	ofstream output("playerdata.dat", ios::binary);
	writePlayerData(output, data);
	output.close();
}

#pragma endregion

	#pragma region rank:排行榜信息

void filename_rank(wchar_t* _filename, int size, int mp, int df)
{
	swprintf_s(_filename, size, L".\\rank\\%d_%d.dat\0", mp, df);
}

void readRankData(ifstream& input, vector<Tuple_Rankinfo> &data)
{
	const int TUPLE_SIZE = sizeof(Tuple_Rankinfo);
	if (input)
	{
		input.seekg(0, ios::end);
		data.resize(input.tellg() / TUPLE_SIZE);
		input.seekg(0, ios::beg);
		input.read(reinterpret_cast<char*>(data.data()), (streampos)(data.size())* (streampos)TUPLE_SIZE);
	}
}

void writeRankData(ofstream& output, const vector<Tuple_Rankinfo> &data)
{
	const int TUPLE_SIZE = sizeof(Tuple_Rankinfo);
	if (output)
	{
		output.seekp(0, ios::beg);
		output.write(reinterpret_cast<const char*>(data.data()), (streampos)(data.size())* (streampos)TUPLE_SIZE);
	}
}

void addRankData(const Options& op, const Score& sc)
{
	if (_access("rank", 0) == -1)	// 若文件夹不存在，创建文件夹
		_mkdir("rank");

	wchar_t file[MAX_PATH];
	filename_rank(file, MAX_PATH, op.mp, op.df);	// 打开对应文件

	vector<Tuple_Rankinfo> data;

	ifstream input(file, ios::binary);
	readRankData(input, data);
	input.close();		// 读

	Tuple_Rankinfo rk(op, sc);
	auto pfind = std::lower_bound(data.begin(), data.end(), rk);
	data.insert(pfind, 1, rk);			// 按序插入

	ofstream output(file, ios::binary);
	writeRankData(output, data);
	output.close();		// 写
}

#pragma endregion

	#pragma region 弃用代码

/*
template<typename Tuple> 
int lower_bound_file(ifstream& input, Tuple target)
{
	const int TUPLE_SIZE = sizeof(Tuple);

	input.seekg(0, ios::end);
	streampos end = input.tellg();
	const int sz = end / TUPLE_SIZE;

	Tuple temp;
	int left = 0, right = sz, mid = 0;
	while (left < right)
	{
		mid = (left + right) / 2;
		input.seekg(mid * TUPLE_SIZE, ios::beg);
		input.read(reinterpret_cast<char*>(&temp), TUPLE_SIZE);
		if (temp < target)
			left = mid + 1;
		else
			right = mid;
	}
	if (mid == sz)
		return -1;
	else
		return mid;
}
*/



/*int search_PlayerData(ifstream& input, const wchar_t* player)
{
	const int TUPLE_SIZE = sizeof(Tuple_Playerinfo);
	//UINT16 mode = 1 << (size_t)(mp * MAXDFCTY + df);

	input.seekg(0, ios::end);
	streampos end = input.tellg();
	const int sz = end / TUPLE_SIZE;
		
	Tuple_Playerinfo temp;
	int left = 0, right = sz, mid = 0;
	int c;
	bool find = false;
	while (left < right)
	{
		mid = (left + right) / 2;
		input.seekg(mid * TUPLE_SIZE, ios::beg);
		input.read(reinterpret_cast<char*>(&temp), TUPLE_SIZE);
		if ((c = wcsncmp(temp.player, player, MAX_NAME)) == 0) {
			find = true;
			break;
		}
		else if (c < 0)
			left = mid + 1;
		else
			right = mid;
	}
	if (find == false)
		return -1;
	else
		return mid;
}*/

/*
class Cmp_Playerinfo
{
public:
	int operator()(const Tuple_Playerinfo& lhs, const Tuple_Playerinfo& rhs)
	{
		return wcsncmp(lhs.player, rhs.player, MAX_NAME);
	}
};
*/


/*
int searchPlayer(const wchar_t* player, Tuple_Playerinfo &info)
{
	ifstream input("playerdata.dat", ifstream::binary);
	if (input)
	{
		const int TUPLE_SIZE = sizeof(Tuple_Playerinfo);
		Tuple_Playerinfo target(player, 0, 0);

		int pos = lower_bound_file<Tuple_Playerinfo>(input, target);
		if (pos != -1)
		{
			input.seekg((streampos)pos * (streampos)TUPLE_SIZE, ios::beg);
			input.read(reinterpret_cast<char*>(&info), TUPLE_SIZE);

			Cmp_Playerinfo comp;
			if (comp(target, info) == 0)
			{
				input.close();
				return (int)info.id;
			}
		}
	}
	input.close();
	return -1;
}
*/

/*
int checkPlayerData(const wchar_t* player, MAPTYPE mp, DFCTY df)
{
	ifstream input("playerdata.dat", ifstream::binary);
	if (input)
	{
		const int TUPLE_SIZE = sizeof(Tuple_Playerinfo);
		Tuple_Playerinfo target(player, mp, df);

		int pos = lower_bound_file<Tuple_Playerinfo>(input, target);
		if (pos != -1)
		{
			input.seekg(pos * TUPLE_SIZE, ios::beg);
			Tuple_Playerinfo base;
			input.read(reinterpret_cast<char*>(&base), TUPLE_SIZE);

			Cmp_Playerinfo comp;
			if ((comp(target, base) == 0) && ((target.mode & base.mode) != 0))
			{
				input.close();
				return (int)base.id;
			}
		}
	}
	input.close();
	return -1;
}
*/
/*
bool addPlayerData(const Options& op)
{
	ofstream output("playerdata.dat", ofstream::binary);
	ifstream input("playerdata.dat", ifstream::binary);
	if (!output || !input) {
		output.close();
		input.close();
		return false;
	}

	const int TUPLE_SIZE = sizeof(Tuple_Playerinfo);

	Tuple_Playerinfo target(op);
	Tuple_Playerinfo base;

	int pos = lower_bound_file<Tuple_Playerinfo>(input, target);

	if (pos != -1)
	{
		input.seekg((streampos)pos * (streampos)TUPLE_SIZE, ios::beg);
		const int cur = input.tellg();

		input.read(reinterpret_cast<char*>(&base), TUPLE_SIZE);
		Cmp_Playerinfo cmp;
		if (cmp(target, base) == 0)	// 用户名相同
		{
			base.mode |= target.mode;
			output.seekp((streampos)pos * (streampos)TUPLE_SIZE, ios::beg);
			output.write(reinterpret_cast<const char*>(&base), TUPLE_SIZE);
		}
		else
		{
			input.seekg(0, ios::end);
			const int end = input.tellg();	// 获取尾后偏移量

			input.seekg(cur, ios::beg);
			output.seekp((streampos)cur + (streampos)TUPLE_SIZE, ios::beg);

			// 后移
			char* temp = new char[end - cur];
			input.read(temp, (long long)end - (long long)cur);
			output.write(temp, (long long)end - (long long)cur);	
			delete []temp;
			
			// 插入
			output.seekp(cur, ios::beg);
			output.write(reinterpret_cast<const char*>(&target), TUPLE_SIZE);
		}
	}
	else
	{
		output.seekp(0, ios::end);
		output.write(reinterpret_cast<char*>(&target), TUPLE_SIZE);
	}
	output.close();
	input.close();
	return true;
}*/

#pragma endregion