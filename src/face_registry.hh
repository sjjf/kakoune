#ifndef face_registry_hh_INCLUDED
#define face_registry_hh_INCLUDED

#include "face.hh"
#include "hash_map.hh"
#include "ranges.hh"
#include "string.hh"
#include "safe_ptr.hh"

namespace Kakoune
{

struct FaceSpec
{
    Face face = {};
    String base = {};

    friend bool operator==(const FaceSpec&, const FaceSpec&) = default;
};

class FaceRegistry : public SafeCountable
{
public:
    FaceRegistry(FaceRegistry& parent) : SafeCountable{}, m_parent(&parent) {}

    void reparent(FaceRegistry& parent) { m_parent = &parent; }

    Face operator[](StringView facedesc) const;
    Face operator[](const FaceSpec& facespec) const;
    void add_face(StringView name, StringView facedesc, bool override = false);
    void remove_face(StringView name);

    using FaceMap = HashMap<String, FaceSpec, MemoryDomain::Faces>;

    auto flatten_faces() const
    {
        auto merge = [](auto&& first, const FaceMap& second) {
            return concatenated(std::forward<decltype(first)>(first)
                                | filter([&second](auto& i) { return not second.contains(i.key); }),
                                second);
        };
        static const FaceMap empty;
        auto& parent = m_parent ? m_parent->m_faces : empty;
        auto& grand_parent = (m_parent and m_parent->m_parent) ? m_parent->m_parent->m_faces : empty;
        return merge(merge(grand_parent, parent), m_faces);
    }

private:
    Face resolve_spec(const FaceSpec& spec) const;

    friend class Scope;
    FaceRegistry();

    SafePtr<FaceRegistry> m_parent;
    FaceMap m_faces;
};

FaceSpec parse_face(StringView facedesc);
String to_string(Face face);

}

#endif // face_registry_hh_INCLUDED
