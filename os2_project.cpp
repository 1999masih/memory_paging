#include<iostream>
#include<string>
#include<fstream>
#include<time.h>
#include<vector>
#include<queue>
#include<chrono>
#include<stack>
#include<math.h>
#include<algorithm>
#include<Windows.h>
#include <iterator>
#include<tuple>
#include <queue>
#include <functional>
#include <random>
#include <sstream>
#include <iterator>

using namespace std;

int max_size_memory = 0, size_page = 0, max_time = 0;



struct process {
	int name = -1;
	int startTime = 0;
	int finishTime = 0;
	int memory_number = 0;
	vector <int> memory;

	process(int name, int startTime, int finishTime, vector<int>memory1) {
		this->name = name;
		this->startTime = startTime;
		this->finishTime = finishTime;
		this->memory = memory1;
		memory_number = memory1.size();
	}
	void set(int name, int startTime, int finishTime, vector<int>memory) {
		this->name = name;
		this->startTime = startTime;
		this->finishTime = finishTime;
		this->memory = memory;
		memory_number = memory.size();
	}

	void printProcess() {
		cout << name << endl;
		cout << startTime << " ";
		cout << finishTime << endl;
		cout << memory.size() << " ";
		for (int j = 0; j < memory.size(); j++)
		{
			cout << memory[j] << " ";
		}
		cout << endl;
	}
	int total_memory() {
		int sum1 = 0;
		for (int j = 0; j < memory.size(); j++)
		{
			sum1 += memory[j];
		}
		return sum1;
	}
};

class main_memory {
public:
	vector<process> mem;

	void print() {
		cout << mem.size() << endl;
		for (int i = 0; i < mem.size(); i++)
		{
			cout << mem[i].name << endl;
			cout << mem[i].startTime << " ";
			cout << mem[i].finishTime << endl;
			cout << mem[i].memory.size() << " ";
			for (int j = 0; j < mem[i].memory.size(); j++)
			{
				cout << mem[i].memory[j] << " ";
			}
			cout << endl;
		}
	}
};

int howMuchMemory(vector<tuple <int, string, string> > memory_map) {
	int freesize = 0;
	for (int i = 0; i < memory_map.size(); i++)
	{
		if (get<0>(memory_map[i]) == 0)
		{
			freesize += size_page;
		}
	}
	return freesize;
}

bool is_file_exist(string fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}
bool is_empty(std::fstream& pFile)
{
	return pFile.peek() == std::fstream::traits_type::eof();
}

vector<process> read_file(string name) {
	ifstream file(name);
	int num = 0;
	int all_process = 0;
	int name_process = 0;
	int startTime = 0;
	int finishTime = 0;
	vector<int> mem;
	vector<process> result;
	string unused; file.clear();
	file.seekg(0, ios::beg);
	if (is_file_exist(name) == true) {
		file >> all_process;
		while (!file.eof()) {
			getline(file, unused);
			if (unused.empty())
			{
				continue;
			}
			name_process = stoi(unused);

			getline(file, unused);
			std::istringstream buf(unused);
			std::istream_iterator<int> beg(buf), end;
			std::vector<int> tokens(beg, end);
			startTime = tokens[0];
			finishTime = tokens[1];

			getline(file, unused);
			std::istringstream buf2(unused);
			std::istream_iterator<int> beg2(buf2), end2;
			std::vector<int> tokens2(beg2, end2);
			tokens2.erase(tokens2.begin() + 0);
			process p(name_process, startTime, finishTime, tokens2);
			result.push_back(p);
		}
	}
	return result;
}

struct Compare {
	bool operator()(process p1, process p2)
	{
		// return "true" if "p1" is ordered  
		// before "p2", for example: 
		return p1.startTime > p2.startTime;
	}
};

void algorithem(main_memory m) {
	ofstream output;
	output.open("output.txt");



	int count_memory = m.mem.size();
	priority_queue<process, vector<process>, Compare> pq;
	vector<process> processes = m.mem;
	vector<tuple <int, string, string> > memory_map;

	output << "time = 0 " << endl;
	for (int i = 0; i < count_memory; i++)
	{
		pq.push(processes[i]);
	}
	priority_queue<process, vector<process>, Compare> pq2;
	pq2 = pq;
	output << "input Queue [";
	while (!pq2.empty())
	{
		output << pq2.top().name << " ->";
		output <<" time(arrive):"<<pq2.top().startTime;
		pq2.pop();
		if (!pq2.empty())
		{
			output << ", ";
		}
	}
	output << "]";
	int total_number_of_pages = max_size_memory / size_page;
	memory_map.resize(total_number_of_pages, tuple<int, string, string>(0, "", ""));

	output << endl << endl;
	/*for (int i = 0; i < memory_map.size(); i++) // print
	{
		cout << get<0>(memory_map[i]) << get<1>(memory_map[i]) << get<2>(memory_map[i]);
	}*/

	while (!pq.empty())
	{
		process newP = pq.top();
		pq2 = pq;

		output << "input Queue [";
		while (!pq2.empty())
		{
			output << pq2.top().name;
			pq2.pop();
			if (!pq2.empty())
			{
				output << ", ";
			}
		}
		output << "]" << endl << endl;

		output << "MM move Process " << pq.top().name << " to memory" << endl;
		pq.pop();
		pq2 = pq;


		output << "input Queue [";
		while (!pq2.empty())
		{
			output << pq2.top().name;
			pq2.pop();
			if (!pq2.empty())
			{
				output << ", ";
			}
		}
		output << "]" << endl;

		output <<endl<< endl;
		output << "Memory Map :" << endl;

		
		if (newP.finishTime - newP.startTime > max_time)
		{
			output << "Process " <<newP.name <<  " cant fit, we cant use this process because its time is bigger than maximum time" << endl;
			output << "it is deleted from the queue." << endl;
			continue;
		}

		// replacment algo.
		if (newP.finishTime - newP.startTime <= max_time && howMuchMemory(memory_map) < newP.total_memory())
		{

			bool f1 = false; 
			for (int i = 0; i < m.mem.size(); i++)
			{
				if (m.mem[i].name != newP.name && m.mem[i].total_memory() + howMuchMemory(memory_map) >= newP.total_memory())
				{
					output << "Process " << newP.name << "(replacement algo.)  -> we chose the best place(memory pages) for this process." << endl;
					f1 = true; // process + free >= new process (replace with 1 process)
					//cout << m.mem[i].total_memory()  << howMuchMemory(memory_map) << " " << newP.total_memory();
					for (int j = 0; j < memory_map.size(); j++)
					{
						if (get<1>(memory_map[j]) == "Process " + to_string(m.mem[i].name)) {
							get<0>(memory_map[j]) = 0;
							get<1>(memory_map[j]) = "";
							get<2>(memory_map[j]) = "";
						}
					}
					break;
				}
			}
			if (!f1) // new process (replace with more than 1 process)
			{
				output << "Process " << newP.name << "(replacement algo.)  -> we chose the best place(memory pages) for this process" << endl <<"\t\t\t\t\t" <<"(multiple processes have been deleted.)" << endl;
				vector<int> name_of_process_that_can_be_deleted;
				int total = howMuchMemory(memory_map);
				for (int i = 0; i < m.mem.size(); i++)
				{
					if (m.mem[i].name != newP.name)
					{
						name_of_process_that_can_be_deleted.push_back(m.mem[i].name);
						total += m.mem[i].total_memory();
					}
					if (total >= newP.total_memory())
					{
						for (int j = 0; j < memory_map.size(); j++)
						{
							for (int z = 0; z < name_of_process_that_can_be_deleted.size(); z++)
							{
								if (get<1>(memory_map[j]) == "Process " + to_string(m.mem[z].name)) {
									get<0>(memory_map[j]) = 0;
									get<1>(memory_map[j]) = "";
									get<2>(memory_map[j]) = "";
								}
							}
						}
						break;
					}
				}
			}
		}
		max_time -= newP.finishTime - newP.startTime;
		if (newP.total_memory() > max_size_memory)
		{
			output << "Process "<< newP.name<<" cant be fitted in whole memory" << endl;
			output << "it is deleted from the queue." << endl;
			continue;
		}
		
		float number_of_page = (float)newP.total_memory() / (float)size_page; // for memory sizes that are not divisble by size of pages.
		
		if (roundf(number_of_page) == number_of_page) // integer
		{
			int count = 1;
			for (int i = 0; i < memory_map.size(); i++)
			{
				if (get<0>(memory_map[i]) == 0 && count <= number_of_page)
				{
					get<0>(memory_map[i]) = size_page;
					get<1>(memory_map[i]) = "Process " + to_string(newP.name);
					get<2>(memory_map[i]) = "Page " + to_string(count++);
				}
			}
		}
		else {// float
			int count = 1;
			int integer = floor(number_of_page);
			float fraction = (float)number_of_page - (float)floor(number_of_page);

			if (integer != 0)
			{
				for (int i = 0; i < memory_map.size(); i++)
				{
					if (get<0>(memory_map[i]) == 0 && count <= integer)
					{
						get<0>(memory_map[i]) = size_page;
						get<1>(memory_map[i]) = "Process " + to_string(newP.name);
						get<2>(memory_map[i]) = "Page " + to_string(count++);
					}
				}
			}
			for (int i = 0; i < memory_map.size(); i++) {
				if (get<0>(memory_map[i]) == 0) {
					get<0>(memory_map[i]) = size_page * fraction;
					get<1>(memory_map[i]) = "Process " + to_string(newP.name);
					get<2>(memory_map[i]) = "Page " + to_string(count++);
					break;
				}
			}
		}
		int memo = 0;
		for (int i = 0; i < memory_map.size(); i++)
		{
			if (get<0>(memory_map[i]) == 0)
			{
				memo += size_page;

				output << memo - size_page << " - " << memo - 1 << " ->" << "free page" << endl;
			}
			else if (get<0>(memory_map[i]) / size_page != 1)
			{
				memo += size_page;
				output << memo - size_page << " - " << memo - 1 << "* ->" << get<1>(memory_map[i]) << " ," << get<2>(memory_map[i]) << endl;
			}
			else {
				memo += size_page;
				output << memo - size_page << " - " << memo - 1 << " ->" << get<1>(memory_map[i]) << " ," << get<2>(memory_map[i]) << endl;
			}
		}
		output << endl << endl;
	}
		output << "* means: that page have internal fragmentation." << endl;
	/*cout << endl;
	int memo = 0;
	for (int i = 0; i < memory_map.size(); i++)
	{
		if (get<0>(memory_map[i]) == 0)
		{
			memo += size_page;
			cout << memo - size_page << " - " << memo - 1 << " ->" << "free page" << endl;
		}
		else if (get<0>(memory_map[i]) /size_page != 1)
		{
			memo += size_page;
			cout << memo - size_page << " - " << memo - 1 << "* ->" << get<1>(memory_map[i]) << " ," << get<2>(memory_map[i]) << endl;
		}
		else {
			memo += size_page;
			cout << memo - size_page << " - " << memo - 1<< " ->" << get<1>(memory_map[i]) << " ," << get<2>(memory_map[i]) << endl;
		}
	}*/
}

int main()
{


	cout << "please enter size of memory :"; cin >> max_size_memory;
	cout << "please enter size of pages :"; cin >> size_page;
	cout << "please enter max time for running :"; cin >> max_time;

	//test:
	/*max_size_memory = 1100;
	size_page = 100;
	max_time = 3500;*/

	main_memory m;
	m.mem = read_file("sample1.txt");
	algorithem(m);
	cout << "please see output file";
	return 0;
}
