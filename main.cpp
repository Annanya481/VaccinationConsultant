#include <iostream> 
#include <string>
#include "sqlite3.h"

using namespace std;
//Callback function
static int callback(void *NotUsed, int count, char **data, char **columns);

//Function to create tables
void CreateTables();

 //Vaccine list displayed
void displayVaccines();

//Insert data into tables
void insertIntoTables();

//Display list of localities
void displayLocalities();

//Display list of sublocalities for option selected
void displaySubLocalities(int ch);

//class user
class User{
    string locality;
    string city;
    string state;
    string phNum;
    string fName;
    string lName;
    string userName;
    string passwd;
    string sql;
    int date;
    int month;
    int year;
public:
    //Function to enter user data
    void input();
    //Function to check valid password (Password must have a minimum length of 8 and must contain alphanumeric characters)
    int checkPassword(string password);
};

int main() 
{
    CreateTables();
    insertIntoTables();

    User U;
    int ch;

    sqlite3* db;
    int exit = 0;
    exit = sqlite3_open("MainDatabase.db", &db);

    string sql;
    char *ErrMsg = 0;

    // Test if there was an error
    if (exit) { 
        cout << "DB Open Error: " << sqlite3_errmsg(db) << endl; 
    } 
    
    //Menu
    cout<<endl<<"*************************************************************************"<<endl;
    cout<<"MENU:\n1.Login\n2.Sign Up\n\n";
    cout<<"*************************************************************************"<<endl;
    cout<<"\nEnter your choice(1-2):  ";
    cin>>ch;

    switch(ch) {
        case 1:{
            string uName, pwd, data;
            cout<<"\nEnter username:  ";
            cin>>uName;
            cout<<"\nEnter password:  ";
            cin>>pwd;
            cout<<endl;
            data = "Welcome " + uName + "!";
            sql = "SELECT * FROM USER WHERE USERNAME = '" + uName + "' AND PASSWORD = '" + pwd + "';";
            exit = sqlite3_exec(db, sql.c_str(), callback, (void*)data.c_str(), &ErrMsg);
            
            if( exit != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", ErrMsg);
                sqlite3_free(ErrMsg);
            } 
            // else {
            //     fprintf(stdout, "Operation okay!\n");
            // }
            break;
        }
        case 2:{
            U.input();
            break;
        }
        default:
            break;
    }

    ch = 0;
    char choice = 'n';
    do{
        cout<<endl<<"*************************************************************************"<<endl;
        cout<<"\n1.Vaccination Information\n2.Closest Healthcentres\n3.Exit\n\n";
        cout<<"*************************************************************************"<<endl;
        cout<<"\nEnter your choice(1-3):  ";
        cin>>ch;

        switch(ch){
            case 1:{
                displayVaccines();
                int vacc_num;
                string data;
                cout<<endl<<"Which vaccine would you like information on (1-9):  ";
                cin>>vacc_num;
                data = "INFORMATION ABOUT VACCINE " + vacc_num;
                sql = "SELECT * FROM VACCINES WHERE ID = " + to_string(vacc_num) + ";";
                exit = sqlite3_exec(db, sql.c_str(), callback, (void*)data.c_str(), &ErrMsg);
                if( exit != SQLITE_OK ){
                    fprintf(stderr, "SQL error: %s\n", ErrMsg);
                    sqlite3_free(ErrMsg);
                }
                break;
            }
            case 2:{
                int ch2;
                string locality;
                displayLocalities();
                cout<<"\nEnter your locality(1-5):  ";
                cin>>ch2;
                displaySubLocalities(ch2);
                cout<<"Enter your sublocality(full name as per displayed menu):  ";
                cin>>locality;
                string data = "INFORMATION ABOUT LOCALITY " + locality;
                sql = "SELECT * FROM CENTRES WHERE LOCALITY = '" + locality + "';";
                exit = sqlite3_exec(db, sql.c_str(), callback, (void*)data.c_str(), &ErrMsg);
                if( exit != SQLITE_OK ){
                    fprintf(stderr, "SQL error: %s\n", ErrMsg);
                    sqlite3_free(ErrMsg);
                }
                break;
            }
            case 3:{
                cout<<endl<<"\nExiting application...\n";
                break;
            }
            default:{
                cout<<"\nWrong choice please try again!\n";
                break;
            }
        } 
        cout<<"\nWould you like to exit(y/n):  ";
        cin>>choice;
    } while(choice == 'n');

    sqlite3_close(db); 
    return 0; 
} 

static int callback(void *var, int count, char **data, char **columns) {
    int i;
    fprintf(stderr, "%s\n ", (const char*)var);
    for(i=0; i<count; i++)
        printf("%s = %s\n", columns[i], data[i] ? data[i] : NULL);
    printf("\n");
    return 0;
}

void CreateTables() {
    sqlite3* db;
    int exit = 0;
    exit = sqlite3_open("MainDatabase.db", &db); 
    char *sql;
    char *ErrMsg = 0;
    // Test if there was an error
    if (exit) { 
        cout << "DB Open Error: " << sqlite3_errmsg(db) << endl; 
    } 

    /*SQL Statement*/
    sql = "CREATE TABLE IF NOT EXISTS USER("  \
          "USERNAME         VARCHAR PRIMARY KEY UNIQUE  NOT NULL,"  \
          "FIRSTNAME        VARCHAR                     NOT NULL,"  \
          "LASTNAME         VARCHAR                     NOT NULL,"  \
          "DATE             INT                         NOT NULL,"  \
          "MONTH            INT                         NOT NULL,"  \
          "YEAR             INT                         NOT NULL,"  \
          "NUMOFVACCINES    INT                         NOT NULL,"  \
          "CITY             VARCHAR                     NOT NULL,"  \
          "STATE            VARCHAR                     NOT NULL,"  \
          "PHONE            INT                         NOT NULL,"  \
          "PASSWORD         VARCHAR                     NOT NULL);";

    /*Execute SQL statement*/
    exit = sqlite3_exec(db, sql, callback, 0, &ErrMsg);

    if( exit != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", ErrMsg);
        sqlite3_free(ErrMsg);
    } 
    
    sql = "CREATE TABLE IF NOT EXISTS CENTRES("  \
          "LOCALITY VARCHAR PRIMARY KEY     NOT NULL,"  \
          "CENTRE   TEXT                    NOT NULL);";

    exit = sqlite3_exec(db, sql, callback, 0, &ErrMsg);

    if( exit != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", ErrMsg);
        sqlite3_free(ErrMsg);
    } 

    sql = "CREATE TABLE IF NOT EXISTS VACCINES("  \
          "ID       INT PRIMARY KEY         NOT NULL,"  \
          "NAME     VARCHAR                 NOT NULL," \
          "DESC     TEXT                    NOT NULL);";
    
    exit = sqlite3_exec(db, sql, callback, 0, &ErrMsg);

    if( exit != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", ErrMsg);
        sqlite3_free(ErrMsg);
    } 
    
    // Close the connection
    sqlite3_close(db); 
}

void User::input(){
    cout<<"\nEnter the following details";

        //firstname
        firstname:
            cout<<"\nFirst Name:  ";
            cin>>fName;

        //lastname
        lastname:
            cout<<"\nLast Name:  ";
            cin>>lName;

        //date of birth
        Date:
            cout<<"\nDate of birth(dd):  ";
            cin>>date;
            cout<<"\nMonth of birth(mm):  ";
            cin>>month;
            cout<<"\nYear of birth(yyyy):  ";
            cin>>year;

        //city
        City:
            cout<<"\nCity:  ";
            cin>>city;
            if(string(city).compare("Bangalore")!=0){
                cout<<"This facility is only for Bangalore. Please try again\n";
                goto City;
            }
        //state
        State:
            cout<<"\nState:  ";
            cin>>state;
            if(string(state).compare("Karnataka")!=0){
                cout<<"This facility is only for Bangalore. Please try again\n";
                goto State;
            }
        //phone number
        ph:
            cout<<"\nPhone number:  ";
            cin>>phNum;
            if(string(phNum).length()!=10){
                    cout<<"Invalid number! Please try again.";
                    goto ph;
            }
        //username
        username:
            cout<<"\nEnter username(min length-4 and max-length-10):  ";
            cin>>userName;
            if(string(userName).length()>10){
                cout<<"Too long! Please try again\n";
                goto username;
            }
            else if(string(userName).length()<4){
                cout<<"Too short! Please try again\n";
                goto username;
            }
        //password
        password:
            cout<<"\nEnter password(please enter an alphanumeric password; min-length 8):  ";
            cin>>passwd;
            if(checkPassword(passwd)==0){
                cout<<"Invalid password! Please try again\n";
                goto password;
            }

        //Insert record into database
        sqlite3* db;
        int exit = 0;
        exit = sqlite3_open("MainDatabase.db", &db); 
        char *ErrMsg = 0;
        if (exit) { 
        cout << "DB Open Error: " << sqlite3_errmsg(db) << endl;  
        } 
        
        sql = "INSERT INTO USER(USERNAME, FIRSTNAME, LASTNAME, DATE, MONTH, YEAR, NUMOFVACCINES, CITY, STATE, PHONE, PASSWORD) VALUES"  \
                "('" + userName + "', '" + fName + "', '" + lName + "', '" + to_string(date) + "', '" + to_string(month) + "', '" \
                + to_string(year) + "', 0, '" + city + "', '" + state + "', '" + phNum + "', '" + passwd + "');";

        exit = sqlite3_exec(db, sql.c_str(), callback, 0, &ErrMsg);
        if(exit == SQLITE_CONSTRAINT_PRIMARYKEY){
            fprintf(stdout, "Username already taken...Please try again!\n");
            goto username;
        }
        if( exit != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", ErrMsg);
            sqlite3_free(ErrMsg);
        } 
        
        sqlite3_close(db);
}

int User::checkPassword(string password){
    int ct=0, cta=0;
    for(auto ch : password){
        if(!isalpha(ch))
            ct++;
        else
            cta++;
    }
    if(ct>0 && cta>0 && password.length()>7)
        return 1;
    return 0;
}

void displayVaccines(){
    cout<<endl<<"*************************************************************************"<<endl;
    cout<<"These are the list of vaccines the child must take."<<endl;
    cout<<"1.Varicella for ChickenPox"<<endl;
    cout<<"2.DTP for Diphtheria, Tetanus and Pertussis"<<endl;
    cout<<"3.Hib for Haemophilius, Influenzae type B"<<endl;
    cout<<"4.Hep A for Hepatitus A"<<endl;
    cout<<"5.Hep B for Hepatitus B"<<endl;
    cout<<"6.MMR for Mumps,Measels,Rubella"<<endl;
    cout<<"7.PCV for Pneumoceal"<<endl;
    cout<<"8.IPV for Polio"<<endl;
    cout<<"9.RV for Rotavirus."<<endl;
    cout<<"*************************************************************************"<<endl;
}

void insertIntoTables() {
    sqlite3* db;
    int exit = 0;
    exit = sqlite3_open("MainDatabase.db", &db); 
    char *sql;
    char *ErrMsg = 0;
    if (exit) { 
        cout << "DB Open Error: " << sqlite3_errmsg(db) << endl; 
    } 
    
    sql = "INSERT INTO VACCINES(ID, NAME, DESC) VALUES" \
          "(1, 'Varicella', 'Varicella is the vaccine required for Chickenpox. The first dose is to be taken after one year and the booster dose "  \
          "after five years.'), "   \
          "(2, 'DTP', 'DTP is the vaccine required for Diptheria, Tetanus and Pertussis. You need to take 5 doses in total. The first three "   \
          "doses are to be taken at 2 months, 4 months, 6 months. The fourth dose can be taken between 15-18 months. The final dose can be taken "  \
           "between 4-6 years.'),"   \
          "(3, 'HiB', 'Hib is for Haemophilius, Influenzae type B. It requires a total of four doses. The first three doses are to be taken at "    \
          "2 months, 4 months, 6 months. The fourth dose can be taken between 12-15 months'),"  \
          "(4, 'Hep A', 'Hep A is for Hepatitus A. Two doses are required, the first dose between 12-23 months and the second dose at least 6 " \
          "months after the first.'),"  \
          "(5, 'Hep B', 'Hep B is for Hepatitus B. It requires a total of three doses. The first dose is to be delivered shortly after birth, " \
           "the second dose between 1-2 months and the final dose between 6-18 months.'),"   \
          "(6, 'MMR', 'MMR is for Mumps,Measels,Rubella. The recommended doses is two. The first dose is to be delivered between 12-15 months and "\
           "the second between 4-6 years.'),"\
           "(7, 'PCV', 'PCV is for Pneumoceal. It requires a total of four doses, one dose at each of these ages: 2 months, 4 months, 6 months, "\
           "and 12 through 15 months.'),"   \
           "(8, 'IPV', 'IPV is for Polio. Two fractional doses are to be given at 6 and 14 weeks.'),"   \
           "(9, 'RV', 'RV is for Rotavirus. Three doses are to be given at 2, 4 and 6 months.') WHERE NOT EXISTS (SELECT * FROM VACCINES);";
    exit = sqlite3_exec(db, sql, callback, 0, &ErrMsg);
    
    sql = "INSERT INTO CENTRES(LOCALITY, CENTRE) VALUES" \
          "('APTS Yelahanka', 'APTS Yelahanka UPHC, Armed Police Training School, Bagaluru Cross - 560063.')," \
          "('Abbigere', 'Abbigere 24/7 UPHC, Kempegowda Gardern, Near Abbigere Park - 560090.')," \
          "('Rajajinagar', 'Rajajinagar UPHC, Near Malleshwaram, Close to Shantinagar.')," \
          "('Mallacandara', 'Mallacandara UPHC, Jalahalli West, MEL Layout')," \
          "('Agara', 'Agara UPHC, 143/3, Saibaba Nagar, Srirampura - 560021.')," \
          "('Viveknagar', 'Viveknagar DISP, Near Anjenya Temple, Main Road - 560047.')," \
          "('Vasanthpura', 'Vasanthpura UPHC, Banashankari, 5th Stage, Subaramanyapura 560078.')," \
          "('Anjanapura', 'Anjanapura UPHC, Anjanapura 2nd Block, Anjanapura Twp - 560062.')," \
          "('Azad Nagar', 'Azad Nagar DISP, Basvanagudi (Head Office) - 560026.')," \
          "('Dodakanahalli', 'Dodakanahalli UPHC, Janath Colony, Chikka Bellandur - 560035.')," \
          "('Koramangala NS', 'Koramangala NS, Koramangala - 560095.')," \
          "('Marathahalli', 'Marathahalli, CT Road, Marathahalli Road - 560037.')," \
          "('Rammamurthynagar', 'Rammamurthynagar UPHC, Kuvempunagar, Doovavaninagar - 560016.')," \
          "('Thanisandra', 'Thanisandra UPHC, Mesthri Palya, Srirampura.')," \
          "('Varthuru', 'Varthuru 24/7 PHC, Balagere Road, Varthuru kere, Dharmaraya Temple - 560082.')," \
          "('Anjanappa Garden', 'Anjanappa Garden HC, Near Balemandi, Binnypet, Bangalore.')," \
          "('Avalahalli', 'Avalahalli H.C., 17th Main, Muneshwara Block, Avalahalli, Bangalore - 85.')," \
          "('Bagalur', 'Bagalur H.C. Old Bagalur Layout, Bangalore.')," \
          "('J. P. Nagar', 'J. P. Nagar HC, 1st Phase, Sarakki, Bangalore - 78.')," \
          "('Kodihalli', 'Kodahalli HC, Next to Maruthi Towers, Airport Road, Bangalore - 08.')," \
          "('Mathikere', 'Mathikere HC, B.K. Nagar, Bangalore - 22.')," \
          "('Cox Town', 'Cox Town UFWC, M. M. Road, Opp. Cox Town Market, Bangalore - 05.')," \
          "('Jayanagar', 'Jayanagar UFWC, 9th Main, 2nd Block, Jayanagar, Bangalore - 11.')," \
          "('Anekal', 'Anekal UPHC, Gh Anekal Premises, Primary Health Centre Anekal, Taluk Office Road, Near Ksrtc Busstand, Anekal Taluk - 562106.')," \
          "('Chandapura', 'Chandapura 24X7 UPHC, Primary Health Centre Chandapura, Dommasandra, Road, Opp. Larves College Of Nursing, Anekal Taluk - 560099')," \
          "('Dommasandra', 'Dommasandra 24X7 UPHC, Sarjapura Main Road, Near Busstand, Anekal Taluk - 562125.')," \
          "('Gudahatti', 'Gudahatti UPHC, Attached To Vetarnary Hospital, Anekal Taluk - 562107.')," \
          "('Hebbagodi', 'Hebbagodi 24X7 UPHC, Bannerugatta & Anekal Main Road, Near Busstand, Anekal Taluk - 560105.')," \
          "('Mahanthalingapura', 'Mahanthalingapura UPHC, Jigani Road, Near Harohalli Road, Anekal Taluk - 560105.')," \
          "('Marsur', 'Marsur UPHC, Main Road, Near Railway Level Cross, Anekal Taluk - 562106.');" ;
    exit = sqlite3_exec(db, sql, callback, 0, &ErrMsg);
    sqlite3_close(db);
}

void displayLocalities() {
    cout<<endl<<"*************************************************************************"<<endl;
    cout<<"1.Bangalore North"<<"\n";
    cout<<"2.Bangalore South"<<"\n";
    cout<<"3.Bangalore East"<<"\n";
    cout<<"4.BBMP"<<"\n";
    cout<<"5.Annekal"<<"\n";
    cout<<"*************************************************************************"<<endl;
}

void displaySubLocalities(int ch) {
    switch(ch){
        case 1:{
            cout<<"1.APTS Yelahanka"<<"\n";
            cout<<"2.Abbigere"<<"\n";
            cout<<"3.Rajajinagar"<<"\n";
            cout<<"4.Mallacandara"<<"\n";
            break;
        }
        case 2:{
            cout<<"1.Agara"<<"\n";
            cout<<"2.Viveknagar"<<"\n";
            cout<<"3.Vasanthpura"<<"\n";
            cout<<"4.Anjanapura"<<"\n";
            break;
        }
        case 3:{
            cout<<"1.Azad Nagar"<<"\n";
            cout<<"2.Dodakanahalli"<<"\n";
            cout<<"3.Koramangala NS"<<"\n";
            cout<<"4.Marathahalli"<<"\n";
            cout<<"5.Rammamurthynagar"<<"\n";
            cout<<"6.Thanisandra"<<"\n";
            cout<<"7.Varthuru"<<"\n";
            break;
        }
        case 4:{
            cout<<"1.Anjanappa Garden"<<"\n";
            cout<<"2.Avalahalli"<<"\n";
            cout<<"3.Bagalur"<<"\n";
            cout<<"4.J. P. Nagar"<<"\n";
            cout<<"5.Kodihalli"<<"\n";
            cout<<"6.Mathikere"<<"\n";
            cout<<"7.Cox Town"<<"\n";
            cout<<"8.Jayanagar"<<"\n";
            break;
        }
        case 5:{
            cout<<"1.Anekal"<<"\n";
            cout<<"2.Chandapura"<<"\n";
            cout<<"3.Dommasandra"<<"\n";
            cout<<"4.Gudahatti"<<"\n";
            cout<<"5.Hebbagodi"<<"\n";
            cout<<"6.Mahanthalingapura"<<"\n";
            cout<<"7.Marsur"<<"\n";
            break;
        }
        default:{
            cout<<"Wrong entry!";
            break;
        }
    }
}