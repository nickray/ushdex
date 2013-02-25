import jinja2
import yaml

from sys import argv

VERBOSE = False

template = """\
#ifndef {{DT}}_RW_H
#define {{DT}}_RW_H

#include "meta_rw.h"

namespace ush {

const char * const {{DT}}_DATA_PREFIX = "{{Dt}}Data::";

struct {{Dt}}Data : public MetaData {
{% for dol, name in members %}
    {{dol}} {{name}};\
{% endfor %}

    {{Dt}}Data() : MetaData() {}

    friend std::ostream & operator<< (std::ostream & o, const {{Dt}}Data & self) {
        o << static_cast<const MetaData &>(self) << ',';

{% for dol, name in members[:-1] %}\
        o << self.{{name}} << ',';
{% if dol == 'double' %}\
        o << hex_dump(self.{{name}}) << ',';
{% endif %}\
{% endfor %}\
{% if members[-1][0]  == 'double' %}\
        o << self.{{members[-1][1]}} << ',';
{% else %}\
        o << self.{{members[-1][1]}};
{% endif %}\
{% if members[-1][0]  == 'double' %}\
        o << hex_dump(self.{{members[-1][1]}});
{% endif %}\
        return o;
    }

    bool operator==(const {{Dt}}Data & other) const {
        return (MetaData::operator==(other) &&
{% for dol, name in members %}\
               ( {{name}} == other.{{name}} ) &&
{% endfor %}\
                true);
    }

};

class {{Dt}}Base : public virtual MetaBase {

    protected:
        {{Dt}}Base(const std::string & rel_contract, const std::string & prefix)
            : MetaBase(rel_contract, prefix)
        {
{% for dol, name in members %}\
            p_{{name}} = locate_{{dol}}_entry("{{name}}");
{% endfor %}\
        }

        // pointers
{% for dol, name in members %}\
        {{dol}} * p_{{name}};
{% endfor %}
};

class {{Dt}}Writer : public MetaWriter<{{Dt}}Writer, {{Dt}}Data>, {{Dt}}Base {

    public:
        {{Dt}}Writer(const std::string & rel_contract)
            : MetaBase(rel_contract, {{DT}}_DATA_PREFIX),
              MetaWriter<{{Dt}}Writer, {{Dt}}Data>(rel_contract, {{DT}}_DATA_PREFIX),
              {{Dt}}Base(rel_contract, {{DT}}_DATA_PREFIX)
        {}

    protected:
        friend class MetaWriter<{{Dt}}Writer, {{Dt}}Data>;
        void write_derived(const {{Dt}}Data & data) {
{% for dol, name in members %}\
            *p_{{name}} = data.{{name}};
{% endfor %}\
        }

};

class {{Dt}}Reader : public MetaReader<{{Dt}}Reader, {{Dt}}Data>, {{Dt}}Base {

    public:
        {{Dt}}Reader(const std::string & rel_contract)
            : MetaBase(rel_contract, {{DT}}_DATA_PREFIX),
              MetaReader<{{Dt}}Reader, {{Dt}}Data>(rel_contract, {{DT}}_DATA_PREFIX), 
              {{Dt}}Base(rel_contract, {{DT}}_DATA_PREFIX)
        {}

    protected:
        friend class MetaReader<{{Dt}}Reader, {{Dt}}Data>;
        void read_derived({{Dt}}Data & data) {
{% for dol, name in members %}\
            data.{{name}} = *p_{{name}};
{% endfor %}\
        }

};

} // namespace ush

#endif // {{DT}}_RW_H
"""

def generate(datatype):
    definition = yaml.load(open(datatype + '.vars')).split()
    members = zip(definition[0::2], definition[1::2])

    dt = datatype.lower()
    Dt = dt.capitalize()
    DT = dt.upper()

    header = jinja2.Template(template).render(**locals())
    if VERBOSE: print header
    open(datatype + '_rw.h', 'w').write(header)

if __name__ == '__main__':
    generate(argv[1])

