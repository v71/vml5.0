#pragma once

#include <vml5.0/strings/stringpath.h>
#include <vml5.0/utils/assetstore.h>
#include <vml5.0/utils/logger2.h>

/////////////////////////////////////////////////////////////////////////////////////////
// SharedResource Node

namespace vml
{
    namespace utils
    {

        class SharedResource
        {
            private:

                friend class SharedResourceStore;

            private:

                // ----------------------------------------------------------------
                // protected data

                int               Instances;
                std::string       ResourceFileName;
                vml::utils::Flags Flags;

            public:

                // ----------------------------------------------------------------
                // copy constructor is private
                // no copies allowed since classes
                // are referenced

                SharedResource(const SharedResource& other) = delete;

                // ----------------------------------------------------------------
                // overload operator = is private
                // no copies allowed since classes
                // are referenced

                SharedResource& operator = (const SharedResource& other) = delete;

                //-----------------------------------------------------------------------------------
                // class is non mmovable

                SharedResource(SharedResource&& other) = delete;
                SharedResource& operator=(SharedResource&& other) = delete;

                // --------------------------------------------------------------------------------
                // get resource filename

                const std::string& GetResourceFileName() const
                {
                    return ResourceFileName;
                }

                // --------------------------------------------------------------------------------
                // flags
                
                const vml::utils::Flags &GetFlags() const 
                {
                    return Flags;
                }
                // --------------------------------------------------------------------------------
                // get resource instances count

                int GetInstancesCount() const
                {
                    return Instances;
                }

                // -----------------------------------------------------------------------
                // dump info

                const std::string Dump()
                {
                    std::string text;
                    text  = std::format("Full path filename : {}\n", ResourceFileName.c_str());
                    text += std::format("File directory     : {}\n", vml::strings::SplitPath::GetDirectory(ResourceFileName).c_str());
                    text += std::format("File extension     : {}\n", vml::strings::SplitPath::GetExtension(ResourceFileName).c_str());
                    text += std::format("File path          : {}\n", vml::strings::SplitPath::GetDrive(ResourceFileName).c_str());
                    text += std::format("Instances          : {}\n", Instances);
                    return text;
                }

                // --------------------------------------------------------------------------------
                // ctor / dtor

                SharedResource(const std::string& key, const vml::utils::Flags& flags)
                {
                    if (key.empty())
                        vml::os::Message::Error("SharedResource instance : Null SharedResource name is not allowed");

                    ResourceFileName = key;
                    Instances        = 0;
                    Flags            = flags;
                }

                virtual ~SharedResource()
                {
                }

        };

    }  // end of namespace SharedResources

} // end of namespace vml


/////////////////////////////////////////////////////////////////////////////////////////
// SharedResource manager

namespace vml
{
    namespace utils
    {

        class SharedResourceStore
        {
            private:

                // --------------------------------------------------------------------------------
                // private data

                std::string Name;
                std::unordered_map<std::string, SharedResource*> Cache;

                // --------------------------------------------------------------------------------
                // delete all cache content

                void ReleaseAll()
                {
                    vml::logger::Logger2::GetInstance()->Info({ "Store","Relasing " + Name });

                    // force release of all elements in the cache

                    for (auto it = Cache.begin(); it != Cache.end(); ++it)
                    {
                        SharedResource* item = it->second;

                        vml::logger::Logger2::GetInstance()->Info({ "Store '" + Name + "'",
                                                                    "Releasing : '"+item->GetResourceFileName()+"' , instances : "+std::to_string(item->GetInstancesCount())});
                        delete it->second;
                    }

                    // clear cache
                    Cache.clear();
                }

            public:

                //---------------------------------------------------------------------
                // copy constructor is private
                // no copies allowed since classes
                // are referenced

                SharedResourceStore(const SharedResourceStore& other) = delete;

                //---------------------------------------------------------------------
                // overload operator is private,
                // no copies allowed since classes
                // are referenced

                SharedResourceStore& operator = (const SharedResourceStore& other) = delete;

                //-----------------------------------------------------------------------------------
                // class is non mmovable

                SharedResourceStore(SharedResourceStore&& other) = delete;
                SharedResourceStore& operator=(SharedResourceStore&& other) = delete;

                // --------------------------------------------------------------------------------
                // adds new element to the cache

                template <class T>
                T* Load(const std::string& fullfilename, const vml::utils::Flags& flags=vml::utils::Flags())
                {
                    if (fullfilename.empty())
                        vml::os::Message::Error("Store : Null filename");

                    // checks if resource is already loaded

                    auto it = Cache.find(fullfilename);

                    // if not, allocate new resource

                    if (it == Cache.end())
                    {
                        // allocate new resource

                        T* item = new T(fullfilename, flags);

                        // insert in cahce

                        Cache.insert(std::pair<std::string, T*>(fullfilename, item));

                        // increase instance counter

                        item->Instances++;

                        // log message out

                        vml::logger::Logger2::GetInstance()->Info({"Store '" + Name + "'",
                                                                   "Adding : '" + fullfilename + "', Instances : " + std::to_string(item->GetInstancesCount())});

                        // return newly creeated item

                        return item;
                    }

                    // if resource is already present, and flags are different, then emit an error

                    if (it->second->Flags!=flags)
                        vml::os::Message::Error("Store : " + Name + " : flags mismatch for same resource '" + fullfilename+"'");

                    // if resource is already present, icrease reference count and return its pointer

                    it->second->Instances++;

                    // log message out

                    vml::logger::Logger2::GetInstance()->Info({"Store '" + Name + "'",
                                                               "Adding : '" + fullfilename + "', Instances : " + std::to_string(it->second->GetInstancesCount()) + " *Already Loaded*" });

                    // return pointer to asset

                    return static_cast<T*>(it->second);
                }

                // --------------------------------------------------------------------------------
                // remove element from database

                bool UnLoad(const std::string& fullfilename)
                {
                    if (fullfilename.empty())
                        vml::os::Message::Error("AssetStore : Null filename when unloading");

                    // checks if resource is loaded 

                    auto it = Cache.find(fullfilename);

                    // if not , erase resource if its
                    // reference values reaches zero

                    if (it != Cache.end())
                    {
                        // cache resource

                        SharedResource* item = it->second;

                        // reduce instances

                        item->Instances--;

                        // log out message
                        
                        vml::logger::Logger2::GetInstance()->Info({"Store '" + Name + "'",
                                                                   "Unloading: '" + fullfilename + "', Instances left : " + std::to_string(item->GetInstancesCount())});

                        // if instances reaches zero, then erase it

                        if (item->Instances == 0)
                        {

                            vml::logger::Logger2::GetInstance()->Info({"Store '" + Name + "'",
                                                                       "Unloading: No more instances for '" + fullfilename + "', Instances : " + std::to_string(item->GetInstancesCount())});
                    
                            // call resource destrctor

                            delete (it->second);

                            // remove resource from cache

                            Cache.erase(it);

                            return true;
                        }

                    }
                    else
                    {
                       vml::os::Message::Error("Store : " + Name + " : Couldn't find : " + fullfilename);
                    }

                    return false;
                }

                // --------------------------------------------------------------------------------
                // dumps content of cache map

                const std::string Dump() const
                {
                    std::string text = Name + " : Dumping cache\n";
                    for (auto it = Cache.begin(); it != Cache.end(); ++it)
                    {
                        SharedResource* item = it->second;
                        text += item->ResourceFileName + " : instances : " + std::to_string(item->Instances) + "\n";
                    }
                    return text;
                }

                // --------------------------------------------------------------------------------
                // gets database name

                const std::string& GetName() const
                {
                    return Name;
                }

                // --------------------------------------------------------------------------------
                // ctor / dtor

                [[nodiscard("AssetStore Without Instance!")]] SharedResourceStore(const std::string& name)
                {
                    // check if we have a valid name
                    if (name.empty())
                        vml::os::Message::Error("Store : Name cannot be empty");

                    // sets SharedResource store name
                    Name = name;

                    // set verbose flag if needed
                    vml::logger::Logger2::GetInstance()->Info({ "Store","Initializing " + Name });
                }

                ~SharedResourceStore()
                {
                    ReleaseAll();
                }

        };

    }  // end of namespace SharedResources

} // end of namespace vml

//////////////////////////////////////////////////////////////////////////////////////////////


/* example of usage
 
	 ////////////////////////////////////////////////////////////////////////

	class A : public vml::utils::SharedResource
	{

		public:

			int a = 1024 * 2;

			A(const std::string& name) :vml::utilss::SharedResource(name)
			{}

			~A()
			{}

	};

	class B : public vml::utils::SharedResource
	{

		public:

		int b = 1024 * 4;

		B(const std::string &name) :vml::utils::SharedResource(name)
		{}

		~B()
		{}

	};

	// code fragment

	vml::utils::SharedResourceStore store;

	A* aptr;
	B* bptr;

	aptr = store.Load<A>("ciao A");
	aptr = store.Load<A>("ciao A_1");
	aptr = store.Load<A>("ciao A");
	aptr = store.Load<A>("ciao A_0");

	bptr = store.Load<B>("ciao B");
	bptr = store.Load<B>("ciao B");
	bptr = store.Load<B>("ciao B_2");

	*/