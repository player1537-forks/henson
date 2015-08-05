#pragma once

/* THE FOLLOWING HAS BEEN AUTOMATICALLY GENERATED BY THE EGG PARSER GENERATOR.
 * DO NOT EDIT. */

// {%


#include <string>
#include <vector>
#include <map>
#include <set>

namespace hwl
{

struct PuppetCommand
{
    std::string     name;
    std::string     command;
};

struct ControlFlow
{
    ControlFlow&    operator=(std::string name_)        { name = name_; return *this; }

    void            add_line(std::string l)                     { commands.push_back(l); }
    void            add_lines(std::vector<std::string> lines)   { commands = lines; }
    void            set_control(std::string c)                  { control = c; }

    std::string                 name;
    std::vector<std::string>    commands;
    std::string                 control;
};

struct Script
{
    void add_puppet(PuppetCommand pc)           { puppets.push_back(pc); }
    void add_procs(ControlFlow cf)              { procs.emplace(cf.name, cf); }

    std::vector<PuppetCommand>                  puppets;
    std::map<std::string, ControlFlow>          procs;
};

}


// %}

#include <string>
#include "parser.hpp"

namespace hwl {

	bool script(parser::state&, Script               &);
	bool puppet(parser::state&, PuppetCommand        &);
	bool command(parser::state&, std::string          &);
	bool procs(parser::state&, ControlFlow          &);
	bool execline(parser::state&, std::string              &);
	bool execlines(parser::state&, std::vector<std::string> &);
	bool name(parser::state&, std::string          &);
	bool letter(parser::state&);
	bool number(parser::state&);
	bool comment(parser::state&);
	bool _(parser::state&);
	bool space(parser::state&);
	bool nl(parser::state&);
	bool eof(parser::state&);

	bool script(parser::state& ps, Script               & psVal) {
		ControlFlow           p;
		PuppetCommand         x;

		return parser::memoize(1, psVal, parser::many(
			parser::choice({
				
					parser::sequence({
						parser::bind(x, puppet),
						[&](parser::state& ps) { psVal.add_puppet(x);  return true; }}),
				
					parser::sequence({
						parser::bind(p, procs),
						[&](parser::state& ps) { psVal.add_procs(p);  return true; }}),
				
					parser::sequence({
						_,
						nl})})))(ps);
	}

	bool puppet(parser::state& ps, PuppetCommand        & psVal) {
		std::string           c;
		std::string           n;

		return parser::memoize(2, psVal, 
			parser::sequence({
				parser::bind(n, name),
				_,
				parser::literal('='),
				_,
				parser::bind(c, command),
				_,
				nl,
				[&](parser::state& ps) { psVal = { n,c };  return true; }}))(ps);
	}

	bool command(parser::state& ps, std::string          & psVal) {
		return parser::memoize(3, psVal, parser::capture(psVal, parser::memoize_many(4, 
			parser::sequence({
				parser::look_not(
					parser::choice({
						nl,
						parser::literal('#')})),
				parser::any()}))))(ps);
	}

	bool procs(parser::state& ps, ControlFlow          & psVal) {
		std::string           c;
		std::vector<std::string>  l;
		std::string           n;

		return parser::memoize(5, psVal, 
			parser::sequence({
				parser::bind(n, name),
				_,
				parser::literal("while"),
				_,
				parser::bind(c, name),
				_,
				parser::literal(':'),
				_,
				nl,
				parser::bind(l, execlines),
				[&](parser::state& ps) { psVal = n; psVal.set_control(c); psVal.add_lines(l);  return true; }}))(ps);
	}

	bool execline(parser::state& ps, std::string              & psVal) {
		return parser::memoize(6, psVal, 
			parser::sequence({
				
					parser::choice({
						parser::literal('\t'),
						parser::literal("    ")}),
				parser::bind(psVal, name),
				_,
				
					parser::choice({
						nl,
						eof})}))(ps);
	}

	bool execlines(parser::state& ps, std::vector<std::string> & psVal) {
		std::string               l;

		return parser::memoize(7, psVal, parser::some(
			parser::choice({
				
					parser::sequence({
						parser::bind(l, execline),
						[&](parser::state& ps) { psVal.push_back(l);  return true; }}),
				
					parser::sequence({
						_,
						nl})})))(ps);
	}

	bool name(parser::state& ps, std::string          & psVal) {
		return parser::memoize(8, psVal, parser::capture(psVal, 
			parser::sequence({
				letter,
				parser::memoize_many(9, 
					parser::choice({
						letter,
						number}))})))(ps);
	}

	bool letter(parser::state& ps) {
		return parser::memoize(10, 
			parser::choice({
				parser::between('a', 'z'),
				parser::between('A', 'Z'),
				parser::literal('_')}))(ps);
	}

	bool number(parser::state& ps) {
		return parser::memoize(11, parser::between('0', '9'))(ps);
	}

	bool comment(parser::state& ps) {
		return parser::memoize(12, 
			parser::sequence({
				parser::literal('#'),
				parser::memoize_many(13, 
					parser::sequence({
						parser::look_not(nl),
						parser::any()}))}))(ps);
	}

	bool _(parser::state& ps) {
		return parser::memoize(14, parser::memoize_many(15, 
			parser::choice({
				space,
				comment})))(ps);
	}

	bool space(parser::state& ps) {
		return parser::memoize(16, 
			parser::choice({
				parser::literal(' '),
				parser::literal('\t')}))(ps);
	}

	bool nl(parser::state& ps) {
		return parser::memoize(17, 
			parser::choice({
				parser::literal("\n\r"),
				parser::literal('\n'),
				parser::literal('\r')}))(ps);
	}

	bool eof(parser::state& ps) {
		return parser::memoize(18, parser::look_not(parser::any()))(ps);
	}

} // namespace hwl

