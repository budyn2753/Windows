// scheduler.cpp

#include "stdafx.h"
#include <iostream>
#include "vector"
#include "list"
#include "conio.h"

unsigned int time = 0;
using namespace std;

class Proces{
public:
	int id;
	int quantum;
	int priority;
	int base_priority;
	unsigned int time_arrive;
	bool finish = false;
	int size;

	Proces(int priority, int time, int size, int id) :
		priority(priority), size(size), id(id), time_arrive(time), quantum(1)
	{
		base_priority = priority;
	};
	
	~Proces(){

	};

	int getTimeArrive() const
	{
		return time_arrive;
	}

	int getPriority() const
	{
		return priority;
	}

	void run(){

		size = size - quantum;
		time_arrive = time;
		cout << "Proces: " << id << " " << "\tPriorytet: " << priority << endl;

		if (quantum == 2){
			cout << "\tGlodzony proces: przydzielony czas 2 kwanty. ";
			quantum = 1;
		}

		if (size <= 0){
			size = 0;
			finish = 1;
			cout << "\tZakonczyl swoje dzialanie." << endl;
		}
		else
			cout << "Pozostaly czas: " << size << endl;

		cout << endl;
	}
};
bool NotEmptyQueue[16];
list<Proces> ReadyProcesQueue[16];
list<Proces> ProcesQueue;
list<Proces>::iterator it;


int FindMaxPriority(){
	for (int i = 15; i > 0; i--){
		if (NotEmptyQueue[i] == 1){
			return i;
		}
	}

	return -1;
}
void SetZeroBitTab(){
	for (int i = 15; i > 0; i--){
		if (ReadyProcesQueue.empty()){
			NotEmptyQueue[i] = 0;
		}
		else {
			NotEmptyQueue[i] = 1;
		}
	}
}
void SetZeroBitTabPriority(int p){
	if (ReadyProcesQueue[p].empty()){
		NotEmptyQueue[p] = 0;
	}
	else {
		NotEmptyQueue[p] = 1;
	}
}
void new_process(int priority, int size, int id, int time_arrive, int time){

	ReadyProcesQueue[priority].push_front(Proces(priority, time_arrive, size, id));
	SetZeroBitTab();
}
void list_print(){
	cout << endl;
	for (int i = 15; i > 0; i--){
		cout << "PRIORYTET: " << i << endl;
		for (it = ReadyProcesQueue[i].begin(); it != ReadyProcesQueue[i].end(); ++it){
			cout << "\tProces: " << it->id << " Priorytet: " << it->priority << " Pozostaly czas: " << it->size << " " << endl;
		}
	}
	cout << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int balance = 0;
	char krok;
	int id = 0;

	cout << "Dodaj proces -a, wyswietl liste -l, nastepny krok -dowolny inny znak: " << endl;

	for (time = 0;; time++){
		krok = _getch();
		if (krok == 'a'){
			int priority;
			int size;
			cout << "Priorytet: ";
			while (!(cin >> priority) || (priority < 0 || priority > 16))
			{
				cout << "Podaj poprawn¹ wartosc priorytetu: ";
				cin.clear();
				fflush(stdin);
			}
			cout << "Dlugosc procesu: ";
			while (!(cin >> size) || size < 0)
			{
				cout << "Podaj poprawna dlugosc procesu: ";
				cin.clear();
				fflush(stdin);
			}
			new_process(priority, size, id, size, time);
			time--;
			id++;
			cin.clear();

		}
		else if (krok == 'l'){
			list_print();
		}

		else {
			int Priority = FindMaxPriority();
			
			if (Priority == -1)
			{
				cout << "Brak procesow" << endl;
				continue;
			}
			
			it = ReadyProcesQueue[Priority].begin();
			it->run();

			if (it->finish == 1){
				ReadyProcesQueue[Priority].erase(it);
				SetZeroBitTabPriority(Priority);
			}
			else if (it->getPriority() != it->base_priority){
				int tmp = it->getPriority();
				int tmp2 = it->priority = it->priority - 1;
				ReadyProcesQueue[it->getPriority()].push_front(*it);
				ReadyProcesQueue[tmp].erase(it);
				SetZeroBitTabPriority(tmp);
				NotEmptyQueue[tmp2] = 1;
			}
			else {
				ReadyProcesQueue[Priority].push_back(*it);
				ReadyProcesQueue[Priority].erase(it);
			}

			balance++;

			if (balance == 5){
				for (int i = 14; i > 0; i--){
					if (NotEmptyQueue[i] == 1){
						for (it = ReadyProcesQueue[i].begin(); it != ReadyProcesQueue[i].end();){
							if ((time - it->time_arrive) >= 9){
									it->priority = 15;
									it->quantum = 2;
									ReadyProcesQueue[15].push_front(*it);
									it = ReadyProcesQueue[i].erase(it);
									SetZeroBitTabPriority(i);
									NotEmptyQueue[15] = 1;
							}
							else
								++it;
						}
					}
				}
				balance = 0;
			}

		}
	}
	return 0;
}