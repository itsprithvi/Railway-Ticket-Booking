#include<iostream>
#include<iomanip>
#include<vector>
#include<queue>
#include<map>
#include<unordered_map>

using namespace std;

class Passenger;
class Do;
class TicketBooker;


class Passenger {
public:
	string name;
	int age;
	char berthPre;
	char alloted;
	int number;
	static int passengerId;

public:

	Passenger(string name, int age, char berthPre) {
		this->name = name;
		this->age = age;
		this->berthPre = berthPre;
		this->passengerId += 1;
		this->alloted = 0;
		/*
		cout << "Name : " << this->name << endl;
		cout << "Age : " << this->age << endl;
		cout << "Birth Preference : " << this->birthPre << endl;
		cout << "Passenger Id : " << this->passengerId << endl;
		*/
	}
};

//Ticket Booker Class
class TicketBooker {
public:
	static int availLowBerth;
	static int availMidBerth;
	static int availUppBerth;
	static int availWait;
	static int availRac;

	static queue<int> waitList;
	static queue<int> racList;
	static vector<int> bookedList;

	static vector<int> lowerListPos;
	static vector<int> middleListPos;
	static vector<int> upperListPos;
	static vector<int> racListPos;
	static vector<int> waitingListPos;

	static map<int, Passenger> passengers;

	friend class Do;
	//to book ticket
	void bookTicket(Passenger p, int seatNo, char berth) {
		p.number = seatNo;
		p.alloted= berth;
		passengers.insert(pair<int, Passenger>(p.passengerId,p));
		//cout << "List size " << passengers.size() << endl;
		bookedList.push_back(p.passengerId);
		cout << "Booked Successfully........  :)" << endl;
		return;
	}

	//add to RAC
	void addToRAC(Passenger p, int seatNo, char berth) {
		p.number = seatNo;
		p.alloted = berth;
		passengers.insert(pair<int, Passenger>(p.passengerId, p));
		racList.push(p.passengerId);
		availRac--;
		racListPos.erase(racListPos.begin());
		return;
	}

	//add to Waiting List
	void addToWaiting(Passenger p, int seatNo, char berth) {
		p.number = seatNo;
		p.alloted = berth;
		passengers.insert(pair<int, Passenger>(p.passengerId, p));
		waitList.push(p.passengerId);
		availWait--;
		waitingListPos.erase(waitingListPos.begin());
		return;
	}
	
	//cancel Ticket
	void cancelTicket(int id) {
		Do d;
		cout << " ended " << endl;
		Passenger pas = passengers.at(id);
		passengers.erase(id);
		remove(bookedList.begin(), bookedList.end(), id);
		int position = pas.number;
		cout << "Cancelled Successfully ... !!!" << endl;

		if (pas.alloted == 'L') {
			availLowBerth++;
			lowerListPos.push_back(position);
		}
		else if (pas.alloted == 'M') {
			availMidBerth++;
			middleListPos.push_back(position);
		}
		else if (pas.alloted == 'U') {
			availUppBerth++;
			upperListPos.push_back(position);
		}

		if (racList.size() > 0) {
			Passenger pasFromRAC = passengers.at(racList.front());
			int posRAC = pasFromRAC.number;
			racListPos.push_back(posRAC);
			racList.pop();
			availRac++;
			if (waitList.size() > 0) {
				Passenger pasFromWL = passengers.at(waitList.front());
				int posWl = pasFromWL.number;
				waitingListPos.push_back(posWl);
				waitList.pop();

				pasFromWL.number = racListPos[0];
				pasFromWL.alloted = 'R';
				racListPos.erase(racListPos.begin());
				racList.push(pasFromWL.passengerId);

				availWait++;
				availRac--;
			}
			d.bookTicket(pasFromRAC);
		}
		return;
	}

	//to print booked tickets
	void printBookedTickets() {
		if (passengers.size() == 0) {
			cout << "There is no Booking started yet .... please book " << endl;
			return;
		}

		for (auto a : passengers) {
			Passenger pas = a.second;
			cout << "Passenger Id : " << a.first << endl;
			cout << "Name : " << pas.name << endl;
			cout << "Age : " << pas.age << endl;
			cout << "Berth : " << pas.alloted << endl;
			cout << endl;
		}
		return;
	}

	//print all available tickets
	void printAvailableTickets() {
		cout << "Lower Berth : " << availLowBerth << endl;
		cout << "Middle Berth : " << availMidBerth << endl;
		cout << "Upper Berth : " << availUppBerth << endl;
		cout << "RAC : " << availRac << endl;
		cout << "Waiting List : " << availWait << endl;
		cout << endl;
	}

};


//Do
class Do {
public:
	//Function to Book Ticket
	void bookTicket(Passenger p) {
		TicketBooker t;
		//If waiting list is zero , then no more seats .....
		if (t.availWait == 0) {
			cout << "Sorry . All Seats Are Filled !" << endl;
		}
		if ((p.berthPre == 'L' && t.availLowBerth > 0) || (p.berthPre == 'M' && t.availMidBerth > 0) || (p.berthPre == 'U' && t.availUppBerth > 0)) {
			cout << "Your Preference is Available :)" << endl;
			if (p.berthPre == 'L') {
				cout << "Lower Berth is Given " << endl;
				t.bookTicket(p, (t.lowerListPos[0]), 'L');
				t.lowerListPos.erase(t.lowerListPos.begin());
				t.availLowBerth--;
			}
			else if (p.berthPre == 'M') {
				cout << "Middle Berth is Given " << endl;
				t.bookTicket(p, (t.middleListPos[0]), 'M');
				t.middleListPos.erase(t.middleListPos.begin());
				t.availMidBerth--;
			}
			else if (p.berthPre == 'U') {
				cout << "Upper Berth is Given " << endl;
				t.bookTicket(p, (t.upperListPos[0]), 'U');
				t.upperListPos.erase(t.upperListPos.begin());
				t.availUppBerth--;
			}
		}
		else if (t.availLowBerth > 0) {
			cout << "Lower Berth is Given " << endl;
			t.bookTicket(p, (t.lowerListPos[0]), 'L');
			t.lowerListPos.erase(t.lowerListPos.begin());
			t.availLowBerth--;
		}
		else if (t.availMidBerth > 0) {
			cout << "Middle Berth is Given " << endl;
			t.bookTicket(p, (t.middleListPos[0]), 'M');
			t.middleListPos.erase(t.middleListPos.begin());
			t.availMidBerth--;
		}
		else if (t.availUppBerth > 0) {
			cout << "Upper Berth is Given " << endl;
			t.bookTicket(p, (t.upperListPos[0]), 'U');
			t.middleListPos.erase(t.upperListPos.begin());
			t.availUppBerth--;
		}
		else if (t.availRac > 0) {
			cout << "RAC is Available " << endl;
			t.addToRAC(p, (t.racListPos[0]), 'R');
		}
		else if (t.availWait > 0) {
			cout << "Added to Waiting List " << endl;
			t.addToWaiting(p, (t.waitingListPos[0]), 'W');
		}

	}
	//Function to Cancel Ticket
	void cancelTicket(int id) {
		TicketBooker t;
		if (t.passengers.find(id) == t.passengers.end()) {
			cout << "This is an Invalid Passenger Id . Please give correct one !" << endl;
			return;
		}
		else {
			cout << "starting" << endl;
			t.cancelTicket(id);
		}
	}

};



//******--Maintaining static data--******//
int Passenger::passengerId = 0;

int TicketBooker::availLowBerth = 1;
int TicketBooker::availMidBerth = 1;
int TicketBooker::availUppBerth = 1;
int TicketBooker::availRac = 1;
int TicketBooker::availWait = 1;

queue<int> TicketBooker::racList;
queue<int> TicketBooker::waitList;

vector<int> TicketBooker::bookedList(0);
vector<int> TicketBooker::lowerListPos = { 1 };
vector<int> TicketBooker::middleListPos = { 1 };
vector<int> TicketBooker::upperListPos = { 1 };
vector<int> TicketBooker::racListPos = { 1 };
vector<int> TicketBooker::waitingListPos = { 1 };

map<int, Passenger> TicketBooker::passengers;

//**********************************************

int main() {
	string name;
	int age;
	char birthPre;
	bool flag = true;
	int id;
	int choice;
	while (flag) {
		cout << "1.Book Ticket" << endl;
		cout << "2.Cancel Ticket" << endl;
		cout << "3.Available Tickets" << endl;
		cout << "4.Booked Tickets" << endl;
		cout << "5.Exit" << endl;
		cin >> choice;
		Do d;
		TicketBooker t;
		switch (choice) {
		case 1:
		{
			cout << "Enter details :" << endl;
			cout << "Enter Name : " << endl;
			cin >> name;
			cout << "Enter Age : " << endl;
			cin >> age;
			cout << "Enter Birth Preference(L, M, U) " << endl;
			cin >> birthPre;
			Passenger p1(name, age, birthPre);
			d.bookTicket(p1);
			cout << endl;
			break;
		}
		case 2:
			cout << "Enter the passenger id to cancel : " << endl;
			cin >> id;
			d.cancelTicket(id);
			cout << endl;
			break;
		case 3:
			t.printAvailableTickets();
			cout << endl;
			break;
		case 4:
			t.printBookedTickets();
			cout << endl;
			break;
		case 5:
			flag = false;
		default: 
			cout << "You Entered an Invalid Option ... please try valid options " << endl;
			cout << endl;
		}

		cout << endl;
	}
	return 0;
}
