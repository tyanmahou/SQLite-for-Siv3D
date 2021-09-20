#pragma once
#include <Siv3D/Blob.hpp>
#include <Siv3D/String.hpp>
#include <Siv3D/HashTable.hpp>
#include <Siv3D/Parse.hpp>

namespace s3dsql
{
    class DBValue;

    using DBRow = s3d::HashTable<s3d::String, DBValue>;
    using DBValueArray = s3d::Array<DBValue>;
    using DBValueMap = s3d::HashTable<s3d::String, DBValue>;

    enum class DBValueType
    {
        Null,
        Int,
        Float,
        String,
        Blob,
    };

    class DBValue
    {
    private:
        DBValueType m_type;

        union
        {
            s3d::int64 i;
            double f;
            s3d::String* str;
            s3d::Blob* blob;
        }m_value;

        void reset(DBValueType type);

        template <class Type, std::enable_if_t<!std::is_arithmetic_v<Type>>* = nullptr>
        s3d::Optional<Type> getOpt_() const
        {
            if (auto str = this->getOptString()) {
                return s3d::ParseOpt<Type>(*str);
            }
            return s3d::none;
        }

        template <class Type, std::enable_if_t<std::is_arithmetic_v<Type>>* = nullptr>
        s3d::Optional<Type> getOpt_() const = delete;

        s3d::Optional<s3d::String> getOptString() const;

        s3d::Optional<s3d::int32> getOptInt32() const;

        s3d::Optional<s3d::uint32> getOptUint32() const;

        s3d::Optional<s3d::int64> getOptInt64() const;

        s3d::Optional<s3d::uint64> getOptUint64() const;

        s3d::Optional<float> getOptFloat() const;

        s3d::Optional<double> getOptDouble() const;

        s3d::Optional<bool> getOptBool() const;

        s3d::Optional<s3d::Blob> getOptBlob() const;

    public:
        DBValue();
        DBValue(const DBValue& other);
        DBValue(DBValue&& other) noexcept;

        DBValue(std::nullptr_t);
        DBValue(s3d::int32 i);
        DBValue(s3d::int64 i);
        DBValue(double f);
        DBValue(const s3d::String& str);
        DBValue(const s3d::String::value_type* str);
        DBValue(const s3d::Blob& blob);

        ~DBValue();

        template <class Type>
        [[nodiscard]] Type get() const
        {
            return getOpt<Type>().value_or(Type());
        }

        template <class Type, class U>
        [[nodiscard]] Type getOr(U&& defaultValue) const
        {
            return getOpt<Type>().value_or(std::forward<U>(defaultValue));
        }

        template <class Type>
        [[nodiscard]] s3d::Optional<Type> getOpt() const
        {
            return getOpt_<Type>();
        }

        [[nodiscard]] DBValueType getType()const;
        [[nodiscard]] bool isNull() const;
        [[nodiscard]] bool isInt() const;
        [[nodiscard]] bool isFloat() const;
        [[nodiscard]] bool isString() const;
        [[nodiscard]] bool isBlob() const;

        void clear();

        DBValue& operator =(const DBValue& other);
        DBValue& operator =(DBValue&& other) noexcept;
    };

    template <>
    [[nodiscard]] inline s3d::Optional<s3d::String> DBValue::getOpt<s3d::String>() const
    {
        return getOptString();
    }

    template <>
    [[nodiscard]] inline s3d::Optional<s3d::int32> DBValue::getOpt<s3d::int32>() const
    {
        return getOptInt32();
    }

    template <>
    [[nodiscard]] inline s3d::Optional<s3d::uint32> DBValue::getOpt<s3d::uint32>() const
    {
        return getOptUint32();
    }

    template <>
    [[nodiscard]] inline s3d::Optional<s3d::int64> DBValue::getOpt<s3d::int64>() const
    {
        return getOptInt64();
    }

    template <>
    [[nodiscard]] inline s3d::Optional<s3d::uint64> DBValue::getOpt<s3d::uint64>() const
    {
        return getOptUint64();
    }

    template <>
    [[nodiscard]] inline s3d::Optional<float> DBValue::getOpt<float>() const
    {
        return getOptFloat();
    }

    template <>
    [[nodiscard]] inline s3d::Optional<double> DBValue::getOpt<double>() const
    {
        return getOptDouble();
    }

    template <>
    [[nodiscard]] inline s3d::Optional<bool> DBValue::getOpt<bool>() const
    {
        return getOptBool();
    }

    template <>
    [[nodiscard]] inline s3d::Optional<s3d::Blob> DBValue::getOpt<s3d::Blob>() const
    {
        return getOptBlob();
    }
}