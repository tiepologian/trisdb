#include <string>
#include <iostream>
#include <map>

template <typename T1, typename T2, typename T3>
class TripleMap {
    public:
        void add(T1 t1, T2 t2, T3 t3) {
	    m1[t1] = std::make_tuple(t1, t2, t3);
	    m2[t2] = std::make_tuple(t1, t2, t3);
	}

	std::tuple<T1,T2,T3> getA(T1 t1) {
            return m1[t1];
	}

	std::tuple<T1,T2,T3> getB(T2 t2) {
	    return m2[t2];
	}

    private:
        std::map<T1,std::tuple<T1,T2,T3>> m1;
	std::map<T2,std::tuple<T1,T2,T3>> m2;
};



int main() {
    TripleMap<std::string,std::string,int> mappa;

//    mappa.add("Gianluca", "Tiepolo", 22);
//    mappa.add("Adele", "Gugliotta", 23);
//    mappa.add("Marco", "Ippolito", 40);
    for(int i=0;i<1000000;i++) {
	mappa.add("Key"+std::to_string(i), "Chiave"+std::to_string(i), i);
    }

//    auto res = mappa.getA("Gianluca");
//    std::cout << std::get<0>(res) << " " << std::get<1>(res) << " - " << std::get<2>(res) << std::endl;

//    auto res2 = mappa.getB("Gugliotta");
//    std::cout << std::get<0>(res2) << " " << std::get<1>(res2) << " - " << std::get<2>(res2) << std::endl;

    std::cout << "COMMAND:" << std::endl;
    while(true) {
        std::string input;
	std::getline(std::cin, input);
	unsigned pos = input.find(" ");
        if(input.substr(0, pos) == "GETA") {
	    // get by name
	    auto res = mappa.getA(input.substr(pos+1));
	    if(std::get<0>(res) == "") std::cout << "NOT FOUND" << std::endl;
    	    else std::cout << std::get<0>(res) << " " << std::get<1>(res) << " - " << std::get<2>(res) << std::endl;
	}
	else if(input.substr(0, pos) == "GETB") {
	    // get by surname
	    auto res2 = mappa.getB(input.substr(pos+1));
	    if(std::get<0>(res2) == "") std::cout << "NOT FOUND" << std::endl;
    	    else std::cout << std::get<0>(res2) << " " << std::get<1>(res2) << " - " << std::get<2>(res2) << std::endl;
	}
    }

    return 0;
}
