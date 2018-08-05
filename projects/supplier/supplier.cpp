#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
using namespace eosio;

class suppliercompany : public eosio::contract {
  public:
      suppliercompany(account_name s):
        contract(s), // initialization of the base class for the contract
        _supplier(s, s) // initialize the table with code and scope NB! Look up definition of code and scope
      {
      }

      /// @abi action
      void creates(account_name username, uint64_t abn, const std::string& fullname, std::string& foodname, uint64_t timeleft, uint64_t qty, uint64_t itemnumber, std::string& notific ) {
        require_auth(username);
        // Let's make sure the primary key doesn't exist
        // _supplier.end() is in a way similar to null and it means that the value isn't found
        eosio_assert(_supplier.find(itemnumber) == _supplier.end(), "This SSN already exists in the addressbook");
        _supplier.emplace(get_self(), [&]( auto& p ) {
           p.abn = abn;
           p.fullname = fullname;
	         p.foodname =foodname;
           p.qty = qty;
	         p.timeleft = timeleft;
           p.itemnumber = itemnumber;
           p.notific = notific;
        });
      }

      /// @abi action
     void update(account_name username, uint64_t abn, const std::string& fullname, std::string& foodname, uint64_t timeleft, uint64_t qty, uint64_t itemnumber, std:: string& notific ) {
     
       require_auth(username);

       auto supplierdist_itr = _supplier.find(itemnumber);

       eosio_assert(supplierdist_itr != _supplier.end(), "Record was not found");

       _supplier.modify( supplierdist_itr, username, [&](auto& p){
           p.abn = abn;
           p.fullname = fullname;
           p.foodname = foodname;
           p.qty = qty;
           p.timeleft = timeleft;
           p.itemnumber = itemnumber;
           p.notific = notific;
        });
     } 
         
  private: 
    // Setup the struct that represents the row in the table
    /// @abi table supplier
    struct supplierdist {
      uint64_t itemnumber; // primary key, social security number
      std::string fullname;
      std::string foodname;
      uint64_t qty;
      uint64_t timeleft;
      uint64_t abn;
      std::string notific;

      uint64_t primary_key()const { return itemnumber; }
      uint64_t by_qty()const { return qty; }
      uint64_t time_left()const { return timeleft; }
      std::string full_name()const { return fullname; }
      std::string food_name()const { return foodname; }
      uint64_t by_abn()const { return abn; }
      std::string notific_s()const { return notific; }
      
    };

    // We setup the table:
    /// @abi table
    typedef eosio::multi_index< N(supplier), supplierdist, indexed_by<N(timeleft), const_mem_fun<supplierdist, uint64_t, &supplierdist::time_left>>>  supplier;

    supplier _supplier;

};

 EOSIO_ABI( suppliercompany, (creates) (update) )
