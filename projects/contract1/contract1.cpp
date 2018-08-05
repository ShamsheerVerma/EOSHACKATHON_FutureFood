#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
using namespace eosio;


class addressbook : public eosio::contract {
  public:
      addressbook(account_name self):
        contract(self), // initialization of the base class for the contract
        _people(_self, _self),
        _people2(_self, _self) // initialize the table with code and scope NB! Look up definition of code and scope
      {
      }

      /// @abi action
      void create(account_name username, uint64_t ssn, const std::string& fullname, uint64_t age) {
        require_auth(username);
        // Let's make sure the primary key doesn't exist
        // _people.end() is in a way similar to null and it means that the value isn't found
        eosio_assert(_people.find(ssn) == _people.end(), "This SSN already exists in the addressbook");
        _people.emplace(get_self(), [&]( auto& p ) {
           p.ssn = ssn;
           p.fullname = fullname;
           p.age = age;
        });
      }

      /// @abi action	
	    void create2(account_name username, uint64_t ssn, const std::string& fullname, uint64_t age) {
        require_auth(username);
        // Let's make sure the primary key doesn't exist
        // _people.end() is in a way similar to null and it means that the valu$
        eosio_assert(_people2.find(ssn) == _people2.end(), "This SSN already exists in the add");
        _people2.emplace(get_self(), [&]( auto& p ) {
           p.ssn = ssn;
           p.fullname = fullname;
           p.age = age;
        });

      } 

  private: 
    // Setup the struct that represents the row in the table
    /// @abi table people
    struct person {
      uint64_t ssn; // primary key, social security number
      std::string fullname;
      uint64_t age;

      uint64_t primary_key()const { return ssn; }
      uint64_t by_age()const { return age; }
    };
    
    typedef eosio::multi_index< N(people), person, indexed_by<N(byage), const_mem_fun<person, uint64_t, &person::by_age>>>  people;
    /// @abi table people2
    struct person2 {
      uint64_t ssn; // primary key, social security number
      std::string fullname;
      uint64_t age;

      uint64_t primary_key()const { return ssn; }
      uint64_t by_age()const { return age; }
    };

    // We setup the table:
    typedef eosio::multi_index< N(people2), person2, indexed_by<N(byage), const_mem_fun<person2, uint64_t, &person2::by_age>>>  people2;

    people _people;
    people2 _people2;
};

 EOSIO_ABI( addressbook, (create) (create2) );
