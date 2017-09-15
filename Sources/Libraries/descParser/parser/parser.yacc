%skeleton "lalr1.cc"
%name-prefix="ObjectParser"
%parse-param {ObjectParser::Lexer& yylex}
%define "parser_class_name" "Parser"
%debug
//%locations
%initial-action
{
	@$.initialize(&yylex.fileName);
};
%error-verbose

%code requires
{

#include <string>
#include "../classDescEtc/grammarComposition.h"
#include "../classDescEtc/classDesc.h"
#include "../classDescEtc/component.h"

#ifdef yylex
#undef yylex
#endif

#ifdef THIS
#undef THIS
#endif


namespace ObjectParser
{
	class Lexer;
};

//#define YYERROR_VERBOSE 1
//#define YYDEBUG 1
}

%union
{
	int intVal;
	float fltVal;
	char strVal[128];
	char name[128];

	Expressions::Expression* expr;
	Expressions::Reference* proxy;
	Expressions::Reference::PathElement* proxyPath;

	Expressions::ConstExprList* guidList;

	PropertyAssignmentList* propertyAssignmentList;
	PropertyAssignment* propertyAssignment;

	ComponentParam* componentParam;
	ComponentParams* componentParams;

	Component* component;
}

%{
	#include "lexer.h"
	#include <iostream>
	#include "../compiler/compiler.h"
%}

%token <name> literal
%token <strVal> str 
%token <strVal> text
%token <fltVal> flt
%token <intVal> digit

%token CLASS			"class definition"
%token INTERFACE		"interface def"
%token COMPONENT		"component definition"
%token INCLUDE			"include"
%token COLLECTION		"collection definition"
%token instance_key 
//%token CONSTRUCTOR		"constructor definition"
%token THISS			"this"
%token VAR	
%token PROPERTY
%token LAMBDA
%token ARRAY
%token NOINSTANCE 
%token true_key 
%token false_key
%token LESS GREATER EQUAL NOT_EQUAL LESS_EQUAL  MORE_EQUAL
%token NOT AND OR XOR
%token PLUS MINUS DIV MOD MUL
%token QUERY COLON 
%token ITERATOR
%token EXTERNAL
%token PRIVATE
%token AGGREGATE		"aggreate inheriatnce"
%token MIX				"mix inheritance"

//expression & var
%type<expr> const
%type<expr> type_def
%type<proxy> proxy
%type<guidList> call_operator
%type<guidList> exp_list
%type<expr> exp
%type<expr> operators
%type<expr> lambda
%type<expr> if_operator
%type<expr> anonymousStruct
%type<expr> external_language
%type<proxy> proxy_head
%type<proxyPath> array_path_element
%type<proxyPath> components_path_element
%type<proxyPath> property_path_element
//component
%type <component> component_def
%type <component> component
%type <componentParam> component_param
%type <componentParams> component_params
%type <propertyAssignment> property_assignment
%type <propertyAssignmentList> instance_body


%left QUERY	
%left AND OR 
%left COMPARISON EQUAL NOT_EQUAL LESS_EQUAL GREATER_EQUAL
%left PLUS MINUS
%left DIV MOD MUL
%left NOT



%expect 43

%%
file: 
	  file_element
	| file file_element
	;

file_element:
	  INCLUDE str		{	yylex.compiler.loadClassDescInfo( boost::filesystem::path(yylex.fileName).parent_path() / $2 );		}
	| class_def			{	yylex.gc.endClass();	}
	| collection_def
	| error
	;


collection_def:
	COLLECTION literal '<' literal '>' '{' collection_var_list '}' 	{ yylex.gc.endCollection($2, $4);	}
	|COLLECTION literal '<' literal '>' '{' '}' 					{ yylex.gc.endCollection($2, $4);	}
	;

collection_var_list:
    collection_element
    | collection_var_list collection_element
    ;

collection_element:
    literal call_operator {	yylex.gc.addCollectionElement($1, $2); }
	;

class_def:  //object
		CLASS literal '{'  '}'							{	yylex.gc.setClassName($2, false);	}
	|	CLASS literal '{' body '}' 						{	yylex.gc.setClassName($2, false);	}
	|	CLASS literal ':' parent_list '{'  '}'			{	yylex.gc.setClassName($2, false);	}
	|	CLASS literal ':' parent_list '{' body '}' 		{	yylex.gc.setClassName($2, false);	}
	|	INTERFACE literal '{'  '}' 						{	yylex.gc.setClassName($2, true);	}
	|	INTERFACE literal '{' body '}'					{	yylex.gc.setClassName($2, true);	}
	|	INTERFACE literal ':' parent_list '{'  '}'		{	yylex.gc.setClassName($2, true);	}
	|	INTERFACE literal ':' parent_list '{' body '}'	{	yylex.gc.setClassName($2, true);	}
	; 

parent_list:
	parent_list ',' inheritanceDef
	| inheritanceDef					{	/*yylex.gc.addParent($1);*/		}
	;

inheritanceDef:
	mixInheritance						{		}
	| aggregateInheritance				{		}		
	;
	
mixInheritance:
	MIX literal							{ yylex.gc.addMixInheriatance($2, nullptr);		}
	| MIX literal '('	')'				{ yylex.gc.addMixInheriatance($2, nullptr);		}
	| MIX literal '(' instance_body ')'	{ yylex.gc.addMixInheriatance($2, std::unique_ptr<PropertyAssignmentList>($4));	}
	;

aggregateInheritance:
	AGGREGATE literal					{ yylex.gc.addAggregateInheritance($2);		}
	;

body: // тело класса
	  element
	| body element
	;

element:
	  component
	| var
	| property_def 
	| instance_def
	| aggregateInheritance_init
	;

aggregateInheritance_init:
	literal '{' '}'					{	}
	| literal '{'instance_body '}'	{	yylex.gc.setInheritanceAssigment($1, std::unique_ptr<PropertyAssignmentList>($3));	}
	;

instance_def:
	  literal literal '{' '}'							{ yylex.gc.newInstance($1, $2, nullptr); }
	| literal literal '{' instance_body '}'				{ yylex.gc.newInstance($1, $2, std::unique_ptr<PropertyAssignmentList>($4)); }
	| literal literal '[' exp ']' '{' '}'				{ yylex.gc.newInstanceArray($1, $2, $4, nullptr); }
	| literal literal '[' exp ']' '{' instance_body '}'	{ yylex.gc.newInstanceArray($1, $2, $4, std::unique_ptr<PropertyAssignmentList>($7)); }
	;

var:
	VAR literal '=' exp 	{	yylex.gc.addvar($2, $4);	}
	;

property_def:
	  PROPERTY literal 						{	yylex.gc.addProperty($2);		}
	| PROPERTY  literal '=' exp 			{	yylex.gc.addProperty($2,$4);	}
	| PRIVATE PROPERTY  literal '=' exp		{	yylex.gc.addProperty($3,$5, true); }
	;

property_assignment:
	  literal '=' exp 		{ $$ = yylex.gc.newPropertyAssignment($1, $3);			/*внутри gc происходит накопление присвоенных свойств*/	}
	| literal '<''-' proxy	{ $$ = 0; yylex.gc.bindRuntimeProperty($1, $4, In);		/*не понятно для чего сейчас нужно, но пока оставил*/	}
	| literal '-''>' proxy	{ $$ = 0; yylex.gc.bindRuntimeProperty($1, $4, Out);	/*не понятно для чего сейчас нужно, но пока оставил*/	}

	;
	
component:	// Component 
	  component_def							{ $$ = yylex.gc.addComponent($1, nullptr); }
	| component_def '{' '}' 				{ $$ = yylex.gc.addComponent($1, nullptr); }
	| component_def '{' instance_body '}'	{ $$ = yylex.gc.addComponent($1, std::unique_ptr<PropertyAssignmentList>($3)); }
	;

component_def:
	  component_runtime_params literal literal							{	$$ = yylex.gc.newComponent($2, $3);																		}
	| component_runtime_params literal literal '(' component_params ')'	{	$$ = yylex.gc.newComponent($2, $3, std::unique_ptr<ComponentParams>($5));								}
	| component_runtime_params literal									{	$$ = yylex.gc.newComponent($2, str::stringize($2, "_", ComponentNameSource::strGuid()));											}
	| component_runtime_params literal '(' component_params ')'			{	$$ = yylex.gc.newComponent($2, str::stringize($2, "_", ComponentNameSource::strGuid()), std::unique_ptr<ComponentParams>($4));	}
	;

component_runtime_params:
	  COMPONENT
	| COMPONENT  '[' ']'					{		}	
	| COMPONENT  '[' component_params ']'	{ yylex.gc.endComponentRuntimeParams(std::unique_ptr<ComponentParams>($3));	}
	;

component_params:
	component_param
	{
		ComponentParams* params = new ComponentParams();
		params->emplace_back($1);
		$$=params;
	}
	| component_params ',' component_param { $$->emplace_back($3); }
	;

component_param:
	literal '=' literal					
	{	
		ComponentParam* param = new ComponentParam($1, $3);
		$$ = param;
	}
	;

exp_list:
	exp					{ $$ = new Expressions::ConstExprList($1); }
	| exp_list ',' exp	{ $1->push_back($3); $$=$1;		}
	;

exp: 
		const				{	$$ = $1;		}
	|	lambda				{	ENFORCE(true);	}
	|	proxy				{	$$ = $1;		}
	|	if_operator			{	$$ = $1;		}
	|	operators			{	$$ = $1;		}
	|	'(' exp ')'			{	$$ = $2;		}
	|	type_def			{	$$ = $1;		}
	|	anonymousStruct		{	$$ = $1;		}
	|   external_language	{	$$ = $1;		}
	|	error{}
	;

type_def:
	literal '{' '}'						{ $$ = yylex.gc.newPrototype($1, nullptr); }
	| literal '{' instance_body '}'		{ $$ = yylex.gc.newPrototype($1, std::unique_ptr<PropertyAssignmentList>($3)); }
	;

instance_body:
	property_assignment						{ $$ = yylex.gc.newPropertyAssignmentsList($1);	   }
	| instance_body property_assignment		{ $$= yylex.gc.addPropertyAssignments($1, $2); }
	| instance_body ',' property_assignment	{ $$= yylex.gc.addPropertyAssignments($1, $3); }
	;


external_language:
	  '#' literal  { $$ = yylex.gc.addExternalExtension($2, std::string() ); }
	;

if_operator:
	exp QUERY exp ':' exp	{ /*$$ = yylex.gc.newTernaryOperator($1, $3, $5);*/ }
	;

operators:
		exp LESS exp %prec COMPARISON 	{ $$ = yylex.gc.newFunction("less",		Expressions::ConstExprList($1, $3));	}
	|	exp GREATER exp	%prec COMPARISON{ $$ = yylex.gc.newFunction("greater",	Expressions::ConstExprList($1, $3));	}
	|	exp LESS_EQUAL exp				{ $$ = yylex.gc.newFunction("lequal",	Expressions::ConstExprList($1, $3));	}
	|	exp GREATER_EQUAL exp			{ $$ = yylex.gc.newFunction("gequal",	Expressions::ConstExprList($1, $3));	}
	|	exp EQUAL exp					{ $$ = yylex.gc.newFunction("equal",	Expressions::ConstExprList($1, $3));	}
	|	exp NOT_EQUAL exp				{ $$ = yylex.gc.newFunction("notequal",	Expressions::ConstExprList($1, $3));	}
	|	exp AND exp						{ $$ = yylex.gc.newFunction("and",		Expressions::ConstExprList($1, $3));	}
	|	exp OR exp						{ $$ = yylex.gc.newFunction("or",		Expressions::ConstExprList($1, $3));	}
	|	NOT exp							{ $$ = yylex.gc.newFunction("not",		Expressions::ConstExprList($2));		}
	|	exp PLUS exp					{ $$ = yylex.gc.newFunction("add", 		Expressions::ConstExprList($1, $3));	}
	|	exp MINUS exp					{ $$ = yylex.gc.newFunction("sub", 		Expressions::ConstExprList($1, $3));	}
	|	exp MUL exp						{ $$ = yylex.gc.newFunction("mul", 		Expressions::ConstExprList($1, $3));	}
	|	exp DIV exp						{ $$ = yylex.gc.newFunction("div", 		Expressions::ConstExprList($1, $3));	}
	|	exp MOD exp						{ $$ = yylex.gc.newFunction("mod", 		Expressions::ConstExprList($1, $3));	}
	;

lambda:
	'['']'literal call_operator		{ /*$$ = yylex.gc.newLambda($3, *$4);*/	}
	;

call_operator:
		'(' exp_list ')'	{	$$ = $2;					}
	|	'(' ')'				{	$$ = new Expressions::ConstExprList();	}
	;


anonymousStruct:
	'{' instance_body '}'  { $$ = yylex.gc.newPropertiesStruct("", std::unique_ptr<PropertyAssignmentList>($2)); }
	;


proxy:
	  proxy_head						{ $$ = $1;}
	| proxy array_path_element			{ $$ = $1; $1->addPathElement($2);	}
	| proxy '.' components_path_element	{ $$ = $1; $1->addPathElement($3);	}
	| proxy '.' property_path_element	{ $$ = $1; $1->addPathElement($3);	}
	;

proxy_head:
	  THISS					{ $$ = yylex.gc.newProxy();					}
	| ITERATOR				{ $$ = yylex.gc.newProxy("iterator");		}
	| literal				{ $$ = yylex.gc.newProxy($1);				}
	;	

array_path_element:
	'[' digit ']'	{ $$ = yylex.gc.newProxyArrayPath($2); }
	;

components_path_element:
	'<' literal '>'			{ $$ = yylex.gc.newProxyComponentPath($2); }
	;

property_path_element:
	literal					{ $$ = yylex.gc.newProxyPropertyPath($1); }
	;


const:
		literal call_operator	{ $$ = yylex.gc.newFunctionOrStruct($1, std::unique_ptr<Expressions::ConstExprList>($2));		}
	|	digit					{ $$ = yylex.gc.newConst($1);						}
	|	str						{ $$ = yylex.gc.newConst(std::string($1));			}
	|	flt						{ $$ = yylex.gc.newConst($1);						}
	|	true_key				{ $$ = yylex.gc.newConst(true);						}
	|	false_key				{ $$ = yylex.gc.newConst(false);					}
	|	'[' exp_list ']'		{ $$ = yylex.gc.newArray(std::unique_ptr<Expressions::ConstExprList>($2));	 					}
	;



%%
void ObjectParser::Parser::error(const location_type& loc, const std::string& msg)
{
	LOG_ERROR("parser error in file \"" <<  yylex.fileName << "\" at: "<< loc << msg);
}
