#include "simplesquirrel/object.hpp"
#include "simplesquirrel/function.hpp"
#include "simplesquirrel/class.hpp"
#include "simplesquirrel/enum.hpp"
#include "simplesquirrel/table.hpp"
#include <squirrel.h>
#include <forward_list>
#include <cassert>
#include <limits>

namespace ssq {
    Table::Table():Object() {
            
    }

    Table::Table(const Object& object):Object(object) {
        if (object.getType() != Type::TABLE) throw TypeException("bad cast", "TABLE", object.getTypeStr());
    }

    Table::Table(HSQUIRRELVM vm):Object(vm) {
        sq_newtable(vm);
        sq_getstackobj(vm, -1, &obj);
        sq_addref(vm, &obj);
        sq_pop(vm,1); // Pop table
    }

    Table::Table(const Table& other):Object(other) {
            
    }

    Table::Table(Table&& other) NOEXCEPT :Object(std::forward<Table>(other)) {
            
    }

    Function Table::findFunc(const char* name) const {
        Object object = Object::find(name);
        return Function(object);
    }

    Class Table::findClass(const char* name) const {
        Object object = Object::find(name);
        return Class(object);
    }

    Table Table::addTable(const char* name) {
        Table table(vm);
        sq_pushobject(vm, obj);
        sq_pushstring(vm, name, strlen(name));
        detail::push<Object>(vm, table);
        sq_newslot(vm, -3, false);
        sq_pop(vm,1); // pop table
        return std::move(table);
    }

    void Table::remove(const char* name) {
      assert(sizeof(name) < static_cast<size_t>(std::numeric_limits<SQInteger>::max()));
      sq_pushobject(vm, obj);
      sq_pushstring(vm, name, static_cast<SQInteger>(strlen(name)));
      sq_deleteslot(vm, -2, SQFalse);
      sq_pop(vm, 1); // pop table
    }

    size_t Table::size() {
        sq_pushobject(vm, obj);
        SQInteger s = sq_getsize(vm, -1);
        sq_pop(vm, 1);
        return static_cast<size_t>(s);
    }

    Table& Table::operator = (const Table& other){
        Object::operator = (other);
        return *this;
    }

    Table& Table::operator = (Table&& other) NOEXCEPT {
        Object::operator = (std::forward<Table>(other));
        return *this;
    }
}
