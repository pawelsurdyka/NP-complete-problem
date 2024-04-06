#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <random>
#include <algorithm>

#include <chrono>
#include <ctime>

using namespace std;

struct Car {
    int ID;
    pair<int, int> start;
    pair<int, int> end;
    string orientation;
    bool forward;
    bool backward ;
    bool has_moved;
    bool operator==(const Car& other) const {
        return ID == other.ID;
    }
};

struct Result {
    int x;
    int y;
    std::string move;
    int distance;
};


int main() {
    int W, H, N;
    cin >> W >> H >> N; // Wczytanie W, H i N
    auto start = std::chrono::system_clock::now();
    // Wczytanie ciągu H wierszy
    vector<string> lines;
    string line;
    getline(cin, line); // Usunięcie znaku nowej linii z bufora

    for (int i = 0; i < H; ++i) {
        getline(cin, line); // Wczytanie kolejnego wiersza
        lines.push_back(line);
    }

    // WxH (szerokość, wysokość)
    vector<Car> cars;

    Car dummy;
    dummy.ID = -1;
    dummy.start = {H-1, 0};
    dummy.end = {H-1, 0};
    dummy.orientation = ".";
    dummy.forward = false;
    dummy.backward = false;
    dummy.has_moved = true;
    cars.push_back(dummy);

    int id = 1;
    pair<int, int> s,e;

    // Samochód dziekana porusza się jedynie w pionie.
    // po prostu zamień kolumny z wierszami

    // auto dziekana
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W ; ++j) { //
            if (lines[i][j] == 'o') {
                int k = 1;
                s = {i,j};
                e = {i,j};
                while (lines[i + k][j] == 'o'){
                    e = {i+k,j};
                    k++;
                }
                Car new_car;
                new_car.ID = id;
                new_car.start = s;
                new_car.end = e;
                new_car.orientation = "|";
                new_car.forward = true;
                new_car.backward = true;
                new_car.has_moved = false;
                cars.push_back(new_car);
                id++;
                break;
            }
        }
        if (id == 2){
            break;
        }
    }

    //
    int dean_column = cars[1].start.second;
    int dean_line = cars[1].start.first;
    //

    // auta pionowe
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W ; ++j) {
            if (lines[i][j] == 'x') {
                s = {i,j};
                e = {i,j};
                if (lines[i+1][j] == 'y'){
                    e = {i+1,j};
                    if (lines[i+2][j] == 'z'){
                        e = {i+2,j};
                        if (lines[i+3][j] == 'w'){
                            e = {i+3,j};
                        }
                    }
                }
                Car new_car;
                new_car.ID = id;
                new_car.start = s;
                new_car.end = e;
                new_car.orientation = "|";
                new_car.forward = true;
                new_car.backward = true;
                new_car.has_moved = false;
                cars.push_back(new_car);
                id++;
            }
        }
    }

    vector<int> nad_dziekan;

    // auta poziome
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W ; ++j) {
            if (lines[i][j] == 'a') {
                s = {i,j};
                e = {i,j};
                if (lines[i][j+1] == 'b'){
                    e = {i,j+1};
                    if (lines[i][j+2] == 'c'){
                        e = {i,j+2};
                        if (lines[i][j+3] == 'd'){
                            e = {i,j+3};
                        }
                    }
                }
                Car new_car;
                new_car.ID = id;
                new_car.start = s;
                new_car.end = e;
                new_car.orientation = "-";
                new_car.forward = true;
                new_car.backward = true;
                new_car.has_moved = false;
                // żeby nie zajeżdzały wyjazdu
                if (new_car.start.second !=  dean_column && new_car.end.second !=  dean_column){
                    if(new_car.start.first < dean_line){
                        new_car.has_moved = true;
                    }
                }
                    // te so zajmują mają czekać na znak
                else{
                    if(new_car.start.first < dean_line){
                        new_car.has_moved = true;
                        nad_dziekan.push_back(new_car.start.first);
                    }
                }
                //
                cars.push_back(new_car);
                id++;
            }
        }
    }

    // mapa z ID aut
    int arr[H][W];
//    int **arr;
//    arr = new int*[H];
//    for (int i = 0; i < H; i++){
//        arr[i] = new int[W];
//    }

    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W ; ++j){
            if (lines[i][j] == '#'){
                arr[i][j] = 0;
            }
            else{
                arr[i][j] = -1;
            }
        }
    }

    for (auto& car : cars) {
        id = car.ID;
        for(int i = car.start.first; i <= car.end.first; i++)
        {
            for(int j = car.start.second; j <= car.end.second; j++)
            {
                arr[i][j] = id;
            }
        }
    }


    // T/F czy mogą się ruszyć
    for (auto& car : cars) {
        if (car.orientation == "|"){
            if(arr[car.start.first-1][car.start.second] >= 0){
                car.forward = false;
            }
            if(arr[car.end.first+1][car.end.second] >= 0){
                car.backward = false;
            }
        }
        else if (car.orientation == "-"){
            if(arr[car.start.first][car.start.second-1] >= 0){
                car.backward = false;
            }
            if(arr[car.end.first][car.end.second+1] >= 0){
                car.forward = false;
            }
        }
    }

    vector<Car> can_move;

    for (auto& car : cars) {
        if (car.forward || car.backward){
            if (!car.has_moved){
                can_move.push_back(car);
            }
        }
    }

    vector<Result> result;

    if (can_move.empty()){
        for (int i = 0; i < nad_dziekan.size();i++){
            string mov;
            if(dean_column == 1){
                mov = "R";
            }
            else{
                mov = "L";
            }
            Result res = { nad_dziekan[i], 2,mov,1};
            result.push_back(res);
        }
        // ruch dziekanem
        Result res = { cars[1].end.first, cars[1].end.second,"U",cars[1].start.first+1};
        result.push_back(res);

        cout << result.size() << endl;
        for (auto& result_moves : result) {
            cout << result_moves.y << " " << result_moves.x << " " <<result_moves.move << " " << result_moves.distance << endl;
        }

        return 0;
    }


    // testowanie rozwiązań
    // reverse move order

    // vector rozwiązań (nadpisuj rozwiązania)

    // kopiowanie

    int arr_c[H][W];

    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W ; ++j){
            arr_c[i][j] = arr[i][j];
        }
    }
    vector<pair<int,int>> starts,ends;

    for (auto& car : cars) {
        starts.push_back(car.start);
        ends.push_back(car.end);
    }
    // jesszcze T/F ruchów

    //
    int moves = 0;
    random_device rd;
    mt19937 gen(rd());

    int id_l, id_r, id_u, id_d;
    //
    int f_nad;
    int move_l, how_far;
    // randomness??

    int gc = -1;

    //


//    for (int i = 0; i < H; ++i) {
//        for (int j = 0; j < W ; ++j){
//            cout << arr[i][j] << "   ";
//        }
//        cout << endl;
//    }
//    cout << endl;
//
//    //
//    for (auto& car : can_move) {
//        cout << car.ID << " - B:" <<car.backward << "F:" << car.forward << "  ";
//    }
//    cout << endl;

    //
    vector<int> forward,backward;
    //
    //
    int flag = 0;
    while (flag == 0){
        // powrót do wartości podstawowych
        // mapa
        for (int i = 0; i < H; ++i) {
            for (int j = 0; j < W ; ++j){
                arr[i][j] = arr_c[i][j];
            }
        }
        // auta
        for (int i = 1; i <= cars.size(); i++) {
            cars[i].start = starts[i];
            cars[i].end = ends[i];
        }
        // T/F ruchów
        for (auto& car : cars) {
            if (car.orientation == "|"){
                if(arr[car.start.first-1][car.start.second] >= 0){
                    car.forward = false;
                }
                if(arr[car.end.first+1][car.end.second] >= 0){
                    car.backward = false;
                }
            }
            else if (car.orientation == "-"){
                if(arr[car.start.first][car.start.second-1] >= 0){
                    car.backward = false;
                }
                if(arr[car.end.first][car.end.second+1] >= 0){
                    car.forward = false;
                }
            }
        }
        // czyszczenie can move i result
        can_move.clear();
        if (!result.empty()){
            cout << result.size() << endl;
            for (auto& r: result){
                cout << r.y << " " << r.x << " " <<r.move << " " << r.distance << endl;
            }
            result.clear();
        }

        // czyszczenie has_moved
        for (auto& car : cars) {
            car.has_moved = false;
            if (car.orientation == "-"){
                if(car.start.first < dean_line){
                    car.has_moved = true;
                }
            }
            if(car.ID == -1 || car.ID == 1){
                car.has_moved = true;
            }
        }

        // ponowne uzupełnienie can_move
        for (auto& car : cars) {
            if (car.forward || car.backward){
                if (!car.has_moved){
                    can_move.push_back(car);
                }
            }
        }


        // additional random

        gc++;
        if (!can_move.empty()){
            gc = gc % can_move.size();
        }
        int obok,min_obok,obok_loop;
        // testowanie rozwiązań
        moves = nad_dziekan.size();
        f_nad = 0;
        while (moves < N) {
            if (!forward.empty()){
                forward.clear();
            }
            if (!backward.empty()){
                backward.clear();
            }
            // sprawdzanie czy auta nad dziekanem mogą odblokować dziekana
            for (int i = 0; i < nad_dziekan.size();i++){
                if (arr[nad_dziekan[i]][4-dean_column] != -1){
                    f_nad = 0;
                    break;
                }
                f_nad = 1;
            }
            if (f_nad == 1){
                for (int i = 0; i < nad_dziekan.size();i++){
                    std::string mov;
                    if(dean_column == 1){
                        mov = "R";
                    }
                    else{
                        mov = "L";
                    }
                    Result res = { nad_dziekan[i], 2,mov,1};
                    result.push_back(res);
                }
                // ruch dziekanem
                Result res = { cars[1].end.first, cars[1].end.second,"U",cars[1].start.first+1};
                result.push_back(res);
                //
//                cout << result.size() << endl;
//                for (auto& result_moves : result) {
//                    cout << result_moves.y << " " << result_moves.x << " " <<result_moves.move << " " << result_moves.distance << endl;
//                }
//
//
//                return 0;

                flag = 1;
                break;
            }

            if (can_move.empty()){
                break;
            }

            move_l = 0, how_far = 1;

            // losowy samochód
            uniform_int_distribution<> distr1(0, can_move.size() - 1);
            int c = distr1(gen);

            //bardzo ciekawe
            if (moves == nad_dziekan.size()){
//                c = gc;
                if (can_move[c].ID == 1){
                    c++;
                    c = c % can_move.size();;
                }
            }
            Car chosen_car = can_move[c];
            int t =0;

            //
            if(2*N < H){
                while (chosen_car.start.first < 3*(N - moves)/2 - 2){
                    c = distr1(gen);
                    chosen_car = can_move[c];
                    t++;
                    if(t > can_move.size()*4){
                        moves = N;
                    }
                }
            }

//            // powiadom te w pobliżu
            t = 0;
            if (!result.empty()){
                while (!(result[result.size()-1].x - 4 < chosen_car.start.first &&
                        chosen_car.start.first < result[result.size()-1].x + 4)){
                    c = distr1(gen);
                    chosen_car = can_move[c];
                    t++;
                    if(t > can_move.size()*4){
                        break;
                    }
                }
            }



            //


            // usunięcię auta
            int cnt = count(can_move.begin(), can_move.end(), chosen_car);

            while (cnt > 0){
                auto it = find_if(can_move.begin(), can_move.end(), [&chosen_car](const Car& car) {
                    return car == chosen_car;
                });
                if (it != can_move.end()) {
                    can_move.erase(it);
                }
                cnt--;
            }

            if(2*N < H){
                if(chosen_car.start.first > N + nad_dziekan.size()){
                    continue;
                }
            }

            // jak juz sie ruszyl
            if(cars[can_move[c].ID].has_moved){
                continue;
            }

            // jak nie może się ruszyć
            if (!chosen_car.forward && !chosen_car.backward){
                continue;
            }

            //// | jeżeli ruch nikogo nie odblokowywuje
//            int far_forward = 0,far_backward = 0;
            int how_f = 1;
            int suma_kontrolna,suma_loop;
            if (chosen_car.orientation == "|"){
                obok = 0;
                suma_kontrolna = 0;
                suma_loop = 0;
                // ile ma sąsiadów zablokowanych -
                for (int i = chosen_car.start.first; i <= chosen_car.end.first; i++){
                    if (arr[i][chosen_car.end.second-1] > 0){
                        if (cars[arr[i][chosen_car.end.second-1]].orientation == "-"){
                            obok++;
                            suma_kontrolna += cars[arr[i][chosen_car.end.second-1]].ID;
                        }
                    }
                    if (arr[i][chosen_car.end.second+1] > 0){
                        if (cars[arr[i][chosen_car.end.second+1]].orientation == "-"){
                            obok++;
                            suma_kontrolna += cars[arr[i][chosen_car.end.second+1]].ID;
                        }
                    }
                }
                min_obok = obok;
                if (chosen_car.backward){
                    while (arr[chosen_car.end.first+how_f][chosen_car.end.second] < 0){
                        how_f++;
                    }
                    how_f--;
                    // sprawdz czy kogoś odblokuje
                    // loop jeden to możliwości , trzeba loop dwa dla testu możliwości
                    for (int i = 0; i <= how_f; i++){
                        obok_loop = 0;
                        suma_loop = 0;
                        for (int j = chosen_car.start.first + i; j <= chosen_car.end.first + i; j++){
                            if (arr[j][chosen_car.end.second-1] > 0){
                                if (cars[arr[j][chosen_car.end.second-1]].orientation == "-"){
                                    obok_loop++;
                                    suma_loop += cars[arr[j][chosen_car.end.second-1]].ID;
                                }
                            }
                            if (arr[j][chosen_car.end.second+1] > 0){
                                if (cars[arr[j][chosen_car.end.second+1]].orientation == "-"){
                                    obok_loop++;
                                    suma_loop += cars[arr[j][chosen_car.end.second+1]].ID;
                                }
                            }
                        }
                        if (min_obok >= obok_loop){
                            min_obok = obok_loop;
                        }
                        if (suma_kontrolna != suma_loop){
                            if (chosen_car.end.first + i < dean_line){
                                if(obok_loop == 0){
                                    backward.push_back(i);
                                }
                            }
                            else{
                                backward.push_back(i);
                            }
//                            backward.push_back(i);
//                            if (min_obok >= obok_loop){
//                                far_backward = i;
//                            }
                        }
                        // jak z nikogo blokuje na nikogo blokuje
                        if(suma_kontrolna == 0 && i != 0){
                            backward.push_back(i);
                        }
                    }
                }
                //
                if (chosen_car.forward){
//                else{
                    how_f = 1;
                    while (arr[chosen_car.start.first - how_f][chosen_car.start.second] < 0
                           && chosen_car.start.first - how_f > 0){
                        how_f++;
                    }
                    how_f--;
                    // sprawdz czy kogoś odblokuje
                    for (int i = 0; i <= how_f; i++){
                        obok_loop = 0;
                        suma_loop = 0;
                        for (int j = chosen_car.start.first - i; j <= chosen_car.end.first - i; j++){
                            if (arr[j][chosen_car.end.second-1] > 0){
                                if (cars[arr[j][chosen_car.end.second-1]].orientation == "-"){
                                    obok_loop++;
                                    suma_loop += cars[arr[j][chosen_car.end.second-1]].ID;
                                }
                            }
                            if (arr[j][chosen_car.end.second+1] > 0){
                                if (cars[arr[j][chosen_car.end.second+1]].orientation == "-"){
                                    obok_loop++;
                                    suma_loop += cars[arr[j][chosen_car.end.second+1]].ID;
                                }
                            }
                        }
                        if (min_obok >= obok_loop){
                            min_obok = obok_loop;
                        }
                        if (suma_kontrolna != suma_loop){
                            if (chosen_car.end.first - i < dean_line){
                                if(obok_loop == 0)
                                    forward.push_back(i);
                            }
                            else{
                                forward.push_back(i);
                            }
//                            forward.push_back(i);
//                            if (min_obok >= obok_loop){
//                                far_forward = i;
//                            }
                        }
                        if(suma_kontrolna == 0 && i != 0){
                            forward.push_back(i);
                        }
                    }
                }
                if (forward.empty() && backward.empty()){
                    continue;
                }
                if (forward.empty()){
                    chosen_car.forward = false;
                }
                if (backward.empty()){
                    chosen_car.backward = false;
                }
            }
            //
            int flip;
            // jak backward i forward == true to 50% na zmienienie backward = false
            if (chosen_car.backward && chosen_car.forward && chosen_car.orientation == "|"){
                if (backward.empty()){
                    chosen_car.backward = false;
                }
                if (forward.empty()){
                    chosen_car.forward = false;
                }
                if (!backward.empty() && !forward.empty()){
                    uniform_int_distribution<> distr2(0, 1);
                    flip = distr2(gen);
                    if(flip == 0){
                        if (chosen_car.orientation == "|"){
                            chosen_car.backward = false;
                        }
                        else{
                            chosen_car.forward = false;
                        }
                    }
                }
            }
            //


            // cos z flopem pomyśl
            //
            ////
            // jak daleko się poruszyć
            if (chosen_car.orientation == "|"){
                if (chosen_car.backward){
                    // jeżeli cos  zablokowało
                    if (arr[chosen_car.end.first+1][chosen_car.end.second] >= 0){
                        chosen_car.backward = false;
                        if (chosen_car.forward){
                            can_move.push_back(chosen_car);
                        }
                        continue;
                    }

                    //
//                    if (backward.empty()){
//                        continue;
//                    }
                    //
                    uniform_int_distribution<> distr3(0, backward.size() - 1);
                    move_l = backward[distr3(gen)];
//                    if(far_backward == 0){
//                        move_l = backward[distr3(gen)];
//                    }
//                    else{
//                        move_l = far_backward;
//                    }
                    // dodanie ruchu do rozwiązania
                    Result res = {chosen_car.start.first,chosen_car.start.second,"D",move_l};
                    moves++;
//                    chosen_car.has_moved = true;
                    cars[chosen_car.ID].has_moved = true;
                    result.push_back(res);
                    for (int i = 0;i < move_l;i++){
                        // informowanie sąsiadów
                        id_l = arr[chosen_car.start.first][chosen_car.start.second-1];
                        id_r = arr[chosen_car.start.first][chosen_car.start.second+1];
                        id_u = arr[chosen_car.start.first-1][chosen_car.start.second];
                        // poruszanie się
                        arr[chosen_car.start.first][chosen_car.start.second] = -1;
                        chosen_car.start.first++;
                        chosen_car.end.first++;
                        arr[chosen_car.end.first][chosen_car.end.second] = chosen_car.ID;


                        // ci mogą się ruszyć

                        if (id_l > 0){
                            Car& neigth = cars[id_l];
                            if (neigth.orientation == "-"  && !neigth.has_moved){
                                neigth.forward = true;
                                can_move.push_back(neigth);
                            }
                        }
                        if (id_r > 0){
                            Car& neigth = cars[id_r];
                            if (neigth.orientation == "-" && !neigth.has_moved){
                                neigth.backward = true;
                                can_move.push_back(neigth);
                            }
                        }
                        if (id_u > 0){
                            Car& neigth = cars[id_u];
                            if (!neigth.has_moved){
                                neigth.backward = true;
                                can_move.push_back(neigth);
                            }
                        }

                    }

                }
                    // forward
                else{
                    // flip reverse
                    if(flip == 0){
                        chosen_car.backward = true;
                    }
                    // jeżeli cos  zablokowało
                    if (arr[chosen_car.start.first-1][chosen_car.end.second] >= 0){
                        chosen_car.forward = false;
                        if (chosen_car.backward){
                            can_move.push_back(chosen_car);
                        }
                        continue;
                    }
//                    if(forward.empty()){
//                        continue;
//                    }
                    uniform_int_distribution<> distr4(0, forward.size() - 1);
                    move_l = forward[distr4(gen)];
//                    if(far_forward == 0){
//                        move_l = forward[distr4(gen)];
//                    }
//                    else{
//                        move_l = far_forward;
//                    }
                    // dodanie ruchu do rozwiązania
                    Result res = {chosen_car.start.first,chosen_car.start.second,"U",move_l};
                    moves++;
//                    chosen_car.has_moved = true;
                    cars[chosen_car.ID].has_moved = true;
                    result.push_back(res);

                    for (int i = 0;i < move_l;i++){
                        // informowanie sąsiadów
                        id_l = arr[chosen_car.end.first][chosen_car.end.second-1];
                        id_r = arr[chosen_car.end.first][chosen_car.end.second+1];
                        id_d = arr[chosen_car.end.first+1][chosen_car.end.second];
                        // poruszanie się
                        arr[chosen_car.end.first][chosen_car.end.second] = -1;
                        chosen_car.end.first--;
                        chosen_car.start.first--;
                        arr[chosen_car.start.first][chosen_car.start.second] = chosen_car.ID;


                        // tym że mogą się poruszyć
                        if (id_l > 0){
                            Car& neigth = cars[id_l];
                            if (neigth.orientation == "-" && !neigth.has_moved){
                                neigth.forward = true;
                                can_move.push_back(neigth);
                            }
                        }
                        if (id_r > 0){
                            Car& neigth = cars[id_r];
                            if (neigth.orientation == "-" && !neigth.has_moved){
                                neigth.backward = true;
                                can_move.push_back(neigth);
                            }
                        }
                        if (id_d > 0){
                            Car& neigth = cars[id_d];
                            if (!neigth.has_moved){
                                neigth.forward = true;
                                can_move.push_back(neigth);
                            }
                        }

                    }
                }
            }
                // "-"
            else{
                if (chosen_car.forward){
                    // jeżeli cos  zablokowało
                    if (arr[chosen_car.end.first][chosen_car.end.second+1] >= 0){
                        chosen_car.forward = false;
                        continue;
                    }
                    // dodanie ruchu do rozwiązania
                    Result res = {chosen_car.start.first,chosen_car.start.second,"R",1};
                    moves++;
//                    chosen_car.has_moved = true;
                    cars[chosen_car.ID].has_moved = true;
                    result.push_back(res);
                    // informowanie sąsiadów
                    id_u = arr[chosen_car.start.first-1][chosen_car.start.second];
                    id_d = arr[chosen_car.start.first+1][chosen_car.start.second];
                    // poruszanie się
                    arr[chosen_car.start.first][chosen_car.start.second] = -1;
                    chosen_car.start.second++;
                    chosen_car.end.second++;
                    arr[chosen_car.end.first][chosen_car.end.second] = chosen_car.ID;

                    // ci mogą się ruszyć

                    if (id_u > 0){
                        Car& neigth = cars[id_u];
                        if (neigth.orientation == "|" && !neigth.has_moved){
                            neigth.backward = true;
                            can_move.push_back(neigth);
                        }
                    }
                    if (id_d > 0){
                        Car& neigth = cars[id_d];
                        if (neigth.orientation == "|" && !neigth.has_moved){
                            neigth.forward = true;
                            can_move.push_back(neigth);
                        }
                    }
                }
                    //backward
                else {
                    if (arr[chosen_car.start.first][chosen_car.start.second-1] >= 0){
                        chosen_car.backward = false;
                        continue;
                    }
                    // dodanie ruchu do rozwiązania
                    Result res = {chosen_car.start.first,chosen_car.start.second,"L",1};
                    moves++;
//                    chosen_car.has_moved = true;
                    cars[chosen_car.ID].has_moved = true;
                    result.push_back(res);
                    // informowanie sąsiadów
                    id_u = arr[chosen_car.end.first-1][chosen_car.end.second];
                    id_d = arr[chosen_car.end.first+1][chosen_car.end.second];
                    // poruszanie się
                    arr[chosen_car.end.first][chosen_car.end.second] = -1;
                    chosen_car.start.second--;
                    chosen_car.end.second--;
                    arr[chosen_car.start.first][chosen_car.start.second] = chosen_car.ID;


                    // ci mogą się ruszyć
                    if (id_u > 0){
                        Car& neigth = cars[id_u];
                        if (neigth.orientation == "|" && !neigth.has_moved){
                            neigth.backward = true;
                            can_move.push_back(neigth);
                        }
                    }
                    if (id_d > 0){
                        Car& neigth = cars[id_d];
                        if (neigth.orientation == "|" && !neigth.has_moved){
                            neigth.forward = true;
                            can_move.push_back(neigth);
                        }
                    }
                }
            }
        }
    }

    // random moves dopuki moves <= N

//    for (int i = 0; i < H; ++i) {
//        for (int j = 0; j < W ; ++j){
//            cout << arr[i][j] << "   ";
//        }
//        cout << endl;
//    }

    // wyniki

    cout << result.size() << endl;
    for (auto& result_moves : result) {
        cout << result_moves.y << " " << result_moves.x << " " <<result_moves.move << " " << result_moves.distance << endl;
    }
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s"
              << std::endl;

    return 0;
}
