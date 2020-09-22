#include <Siv3DSQL/DBValue.hpp>
#include <Siv3D/Parse.hpp>

namespace s3dsql
{
    DBValue::DBValue():
        m_type(DBValueType::Null)
    {
        memset(&m_value, 0, sizeof(m_value));
    }
    DBValue::DBValue(const DBValue & other) :
        m_type(DBValueType::Null)
    {
        *this = other;
    }
    DBValue::DBValue(DBValue && other) noexcept :
        m_type(DBValueType::Null)
    {
        *this = std::move(other);
    }
    DBValue::DBValue(std::nullptr_t):
        m_type(DBValueType::Null)
    {}
    DBValue::DBValue(s3d::int32 i):
        m_type(DBValueType::Int)
    {
        m_value.i = static_cast<s3d::int64>(i);
    }
    DBValue::DBValue(s3d::int64 i):
        m_type(DBValueType::Int)
    {
        m_value.i = i;
    }
    DBValue::DBValue(double f) :
        m_type(DBValueType::Float)
    {
        m_value.f = f;
    }
    DBValue::DBValue(const s3d::String & str) :
        m_type(DBValueType::String)
    {
        m_value.str = new s3d::String(str);
    }
    DBValue::DBValue(const s3d::ByteArray& blob) :
        m_type(DBValueType::Blob)
    {
        m_value.blob = new s3d::ByteArray(blob);
    }
    DBValue::~DBValue()
    {
        this->clear();
    }
    DBValueType DBValue::getType() const
    {
        return m_type;
    }
    bool DBValue::isNull() const
    {
        return m_type == DBValueType::Null;
    }
    bool DBValue::isInt() const
    {
        return m_type == DBValueType::Int;
    }
    bool DBValue::isFloat() const
    {
        return m_type == DBValueType::Float;
    }
    bool DBValue::isString() const
    {
        return m_type == DBValueType::String;
    }
    bool DBValue::isBlob() const
    {
        return m_type == DBValueType::Blob;
    }
    void DBValue::clear()
    {
        switch (this->m_type) {
        case DBValueType::Int:
            this->m_value.i = 0;
            break;
        case DBValueType::Float:
            this->m_value.f = 0.0;
            break;
        case DBValueType::String:
            delete this->m_value.str;
            this->m_value.str = nullptr;
            break;
        case DBValueType::Blob:
            delete this->m_value.blob;
            this->m_value.blob = nullptr;
            break;
        default:
            break;
        }
        this->m_type = DBValueType::Null;
    }
    void DBValue::reset(DBValueType type)
    {
        if (m_type == type) {
            return;
        }
        this->clear();
        switch (type) {
        case DBValueType::String:
            m_value.str = new (std::nothrow) s3d::String();
            break;
        case DBValueType::Blob:
            m_value.blob = new (std::nothrow) s3d::ByteArray();
            break;
        default:
            break;
        }
        m_type = type;
    }
    s3d::Optional<s3d::String> DBValue::getOptString() const
    {
        switch (this->m_type) {
        case DBValueType::Int:
            return s3d::Format(this->m_value.i);
        case DBValueType::Float:
            return s3d::Format(this->m_value.f);
        case DBValueType::String:
            return *m_value.str;
        default:
            return s3d::none;
        }
    }
    s3d::Optional<s3d::int32> DBValue::getOptInt32() const
    {
        switch (this->m_type) {
        case DBValueType::Int:
            return static_cast<s3d::int32>(this->m_value.i);
        case DBValueType::Float:
            return static_cast<s3d::int32>(this->m_value.f);
        case DBValueType::String:
            return s3d::ParseOpt<s3d::int32>(*m_value.str);
        default:
            return s3d::none;
        }
    }
    s3d::Optional<s3d::uint32> DBValue::getOptUint32() const
    {
        switch (this->m_type) {
        case DBValueType::Int:
            return static_cast<s3d::uint32>(this->m_value.i);
        case DBValueType::Float:
            return static_cast<s3d::uint32>(this->m_value.f);
        case DBValueType::String:
            return s3d::ParseOpt<s3d::uint32>(*m_value.str);
        default:
            return s3d::none;
        }
    }
    s3d::Optional<s3d::int64> DBValue::getOptInt64() const
    {
        switch (this->m_type) {
        case DBValueType::Int:
            return this->m_value.i;
        case DBValueType::Float:
            return static_cast<s3d::int64>(this->m_value.f);
        case DBValueType::String:
            return s3d::ParseOpt<s3d::int64>(*m_value.str);
        default:
            return s3d::none;
        }
    }
    s3d::Optional<s3d::uint64> DBValue::getOptUint64() const
    {
        switch (this->m_type) {
        case DBValueType::Int:
            return static_cast<s3d::uint64>(this->m_value.i);
        case DBValueType::Float:
            return static_cast<s3d::uint64>(this->m_value.f);
        case DBValueType::String:
            return s3d::ParseOpt<s3d::uint64>(*m_value.str);
        default:
            return s3d::none;
        }
    }
    s3d::Optional<float> DBValue::getOptFloat() const
    {
        switch (this->m_type) {
        case DBValueType::Int:
            return static_cast<float>(this->m_value.i);
        case DBValueType::Float:
            return static_cast<float>(this->m_value.f);
        case DBValueType::String:
            return s3d::ParseOpt<float>(*m_value.str);
        default:
            return s3d::none;
        }
    }
    s3d::Optional<double> DBValue::getOptDouble() const
    {
        switch (this->m_type) {
        case DBValueType::Int:
            return static_cast<double>(this->m_value.i);
        case DBValueType::Float:
            return this->m_value.f;
        case DBValueType::String:
            return s3d::ParseOpt<double>(*m_value.str);
        default:
            return s3d::none;
        }
    }
    s3d::Optional<bool> DBValue::getOptBool() const
    {
        switch (this->m_type) {
        case DBValueType::Int:
            return this->m_value.i == 0 ? false : true;
        case DBValueType::Float:
            return this->m_value.f == 0.0 ? false : true;
        case DBValueType::String:
            return s3d::ParseOpt<bool>(*m_value.str);
        default:
            return s3d::none;
        }
    }
    s3d::Optional<s3d::ByteArray> DBValue::getOptByteArray() const
    {
        if (!this->isBlob()) {
            return s3d::none;
        }
        return *m_value.blob;
    }
    DBValue& DBValue::operator=(const DBValue& other)
    {
        if (this == &other) {
            return *this;
        }
        this->reset(other.m_type);

        switch (other.m_type) {
        case DBValueType::Int:
            this->m_value.i = other.m_value.i;
            break;
        case DBValueType::Float:
            this->m_value.f = other.m_value.f;
            break;
        case DBValueType::String:
            if (this->m_value.str == nullptr) {
                this->m_value.str = new s3d::String();
            }
            *this->m_value.str = *other.m_value.str;
            break;
        case DBValueType::Blob:
            if (this->m_value.blob == nullptr) {
                this->m_value.blob = new s3d::ByteArray();
            }
            *this->m_value.blob = *other.m_value.blob;
            break;
        default:
            break;
        }
        return *this;
    }
    DBValue& DBValue::operator=(DBValue&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }
        this->clear();
        switch (other.m_type) {
        case DBValueType::Int:
            this->m_value.i = other.m_value.i;
            break;
        case DBValueType::Float:
            this->m_value.f = other.m_value.f;
            break;
        case DBValueType::String:
            this->m_value.str = other.m_value.str;
            break;
        case DBValueType::Blob:
            this->m_value.blob = other.m_value.blob;
            break;
        default:
            break;
        }
        this->m_type = other.m_type;
        other.m_type = DBValueType::Null;
        memset(&other.m_value, 0, sizeof(other.m_value));

        return *this;
    }
}