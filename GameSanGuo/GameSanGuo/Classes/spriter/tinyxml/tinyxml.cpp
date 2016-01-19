/*
www.sourceforge.net/projects/tinyxml
Original code (2.0 and earlier )copyright (c) 2000-2002 Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#include <ctype.h>
#include "tinyxml.h"

#ifdef TTiXml_USE_STL
#include <sstream>
#include <iostream>
#endif


bool TTiXmlBase::condenseWhiteSpace = true;

void TTiXmlBase::PutString( const TTiXml_STRING& str, TTiXml_OSTREAM* stream )
{
	TTiXml_STRING buffer;
	PutString( str, &buffer );
	(*stream) << buffer;
}

void TTiXmlBase::PutString( const TTiXml_STRING& str, TTiXml_STRING* outString )
{
	int i=0;

	while( i<(int)str.length() )
	{
		unsigned char c = (unsigned char) str[i];

		if (    c == '&' 
		     && i < ( (int)str.length() - 2 )
			 && str[i+1] == '#'
			 && str[i+2] == 'x' )
		{
			// Hexadecimal character reference.
			// Pass through unchanged.
			// &#xA9;	-- copyright symbol, for example.
			//
			// The -1 is a bug fix from Rob Laveaux. It keeps
			// an overflow from happening if there is no ';'.
			// There are actually 2 ways to exit this loop -
			// while fails (error case) and break (semicolon found).
			// However, there is no mechanism (currently) for
			// this function to return an error.
			while ( i<(int)str.length()-1 )
			{
				outString->append( str.c_str() + i, 1 );
				++i;
				if ( str[i] == ';' )
					break;
			}
		}
		else if ( c == '&' )
		{
			outString->append( entity[0].str, entity[0].strLength );
			++i;
		}
		else if ( c == '<' )
		{
			outString->append( entity[1].str, entity[1].strLength );
			++i;
		}
		else if ( c == '>' )
		{
			outString->append( entity[2].str, entity[2].strLength );
			++i;
		}
		else if ( c == '\"' )
		{
			outString->append( entity[3].str, entity[3].strLength );
			++i;
		}
		else if ( c == '\'' )
		{
			outString->append( entity[4].str, entity[4].strLength );
			++i;
		}
		else if ( c < 32 )
		{
			// Easy pass at non-alpha/numeric/symbol
			// Below 32 is symbolic.
			char buf[ 32 ];
			
			#if defined(TTiXml_SNPRINTF)		
				TTiXml_SNPRINTF( buf, sizeof(buf), "&#x%02X;", (unsigned) ( c & 0xff ) );
			#else
				sprintf( buf, "&#x%02X;", (unsigned) ( c & 0xff ) );
			#endif		

			//*ME:	warning C4267: convert 'size_t' to 'int'
			//*ME:	Int-Cast to make compiler happy ...
			outString->append( buf, (int)strlen( buf ) );
			++i;
		}
		else
		{
			//char realc = (char) c;
			//outString->append( &realc, 1 );
			*outString += (char) c;	// somewhat more efficient function call.
			++i;
		}
	}
}


// <-- Strange class for a bug fix. Search for STL_STRING_BUG
TTiXmlBase::StringToBuffer::StringToBuffer( const TTiXml_STRING& str )
{
	buffer = new char[ str.length()+1 ];
	if ( buffer )
	{
		strcpy( buffer, str.c_str() );
	}
}


TTiXmlBase::StringToBuffer::~StringToBuffer()
{
	delete [] buffer;
}
// End strange bug fix. -->


TTiXmlNode::TTiXmlNode( NodeType _type ) : TTiXmlBase()
{
	parent = 0;
	type = _type;
	firstChild = 0;
	lastChild = 0;
	prev = 0;
	next = 0;
}


TTiXmlNode::~TTiXmlNode()
{
	TTiXmlNode* node = firstChild;
	TTiXmlNode* temp = 0;

	while ( node )
	{
		temp = node;
		node = node->next;
		delete temp;
	}	
}


void TTiXmlNode::CopyTo( TTiXmlNode* target ) const
{
	target->SetValue (value.c_str() );
	target->userData = userData; 
}


void TTiXmlNode::Clear()
{
	TTiXmlNode* node = firstChild;
	TTiXmlNode* temp = 0;

	while ( node )
	{
		temp = node;
		node = node->next;
		delete temp;
	}	

	firstChild = 0;
	lastChild = 0;
}


TTiXmlNode* TTiXmlNode::LinkEndChild( TTiXmlNode* node )
{
	assert( node->parent == 0 || node->parent == this );
	assert( node->GetDocument() == 0 || node->GetDocument() == this->GetDocument() );

	node->parent = this;

	node->prev = lastChild;
	node->next = 0;

	if ( lastChild )
		lastChild->next = node;
	else
		firstChild = node;			// it was an empty list.

	lastChild = node;
	return node;
}


TTiXmlNode* TTiXmlNode::InsertEndChild( const TTiXmlNode& addThis )
{
	TTiXmlNode* node = addThis.Clone();
	if ( !node )
		return 0;

	return LinkEndChild( node );
}


TTiXmlNode* TTiXmlNode::InsertBeforeChild( TTiXmlNode* beforeThis, const TTiXmlNode& addThis )
{	
	if ( !beforeThis || beforeThis->parent != this )
		return 0;

	TTiXmlNode* node = addThis.Clone();
	if ( !node )
		return 0;
	node->parent = this;

	node->next = beforeThis;
	node->prev = beforeThis->prev;
	if ( beforeThis->prev )
	{
		beforeThis->prev->next = node;
	}
	else
	{
		assert( firstChild == beforeThis );
		firstChild = node;
	}
	beforeThis->prev = node;
	return node;
}


TTiXmlNode* TTiXmlNode::InsertAfterChild( TTiXmlNode* afterThis, const TTiXmlNode& addThis )
{
	if ( !afterThis || afterThis->parent != this )
		return 0;

	TTiXmlNode* node = addThis.Clone();
	if ( !node )
		return 0;
	node->parent = this;

	node->prev = afterThis;
	node->next = afterThis->next;
	if ( afterThis->next )
	{
		afterThis->next->prev = node;
	}
	else
	{
		assert( lastChild == afterThis );
		lastChild = node;
	}
	afterThis->next = node;
	return node;
}


TTiXmlNode* TTiXmlNode::ReplaceChild( TTiXmlNode* replaceThis, const TTiXmlNode& withThis )
{
	if ( replaceThis->parent != this )
		return 0;

	TTiXmlNode* node = withThis.Clone();
	if ( !node )
		return 0;

	node->next = replaceThis->next;
	node->prev = replaceThis->prev;

	if ( replaceThis->next )
		replaceThis->next->prev = node;
	else
		lastChild = node;

	if ( replaceThis->prev )
		replaceThis->prev->next = node;
	else
		firstChild = node;

	delete replaceThis;
	node->parent = this;
	return node;
}


bool TTiXmlNode::RemoveChild( TTiXmlNode* removeThis )
{
	if ( removeThis->parent != this )
	{	
		assert( 0 );
		return false;
	}

	if ( removeThis->next )
		removeThis->next->prev = removeThis->prev;
	else
		lastChild = removeThis->prev;

	if ( removeThis->prev )
		removeThis->prev->next = removeThis->next;
	else
		firstChild = removeThis->next;

	delete removeThis;
	return true;
}

const TTiXmlNode* TTiXmlNode::FirstChild( const char * _value ) const
{
	const TTiXmlNode* node;
	for ( node = firstChild; node; node = node->next )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}


TTiXmlNode* TTiXmlNode::FirstChild( const char * _value )
{
	TTiXmlNode* node;
	for ( node = firstChild; node; node = node->next )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}


const TTiXmlNode* TTiXmlNode::LastChild( const char * _value ) const
{
	const TTiXmlNode* node;
	for ( node = lastChild; node; node = node->prev )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}

TTiXmlNode* TTiXmlNode::LastChild( const char * _value )
{
	TTiXmlNode* node;
	for ( node = lastChild; node; node = node->prev )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}

const TTiXmlNode* TTiXmlNode::IterateChildren( const TTiXmlNode* previous ) const
{
	if ( !previous )
	{
		return FirstChild();
	}
	else
	{
		assert( previous->parent == this );
		return previous->NextSibling();
	}
}

TTiXmlNode* TTiXmlNode::IterateChildren( TTiXmlNode* previous )
{
	if ( !previous )
	{
		return FirstChild();
	}
	else
	{
		assert( previous->parent == this );
		return previous->NextSibling();
	}
}

const TTiXmlNode* TTiXmlNode::IterateChildren( const char * val, const TTiXmlNode* previous ) const
{
	if ( !previous )
	{
		return FirstChild( val );
	}
	else
	{
		assert( previous->parent == this );
		return previous->NextSibling( val );
	}
}

TTiXmlNode* TTiXmlNode::IterateChildren( const char * val, TTiXmlNode* previous )
{
	if ( !previous )
	{
		return FirstChild( val );
	}
	else
	{
		assert( previous->parent == this );
		return previous->NextSibling( val );
	}
}

const TTiXmlNode* TTiXmlNode::NextSibling( const char * _value ) const 
{
	const TTiXmlNode* node;
	for ( node = next; node; node = node->next )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}

TTiXmlNode* TTiXmlNode::NextSibling( const char * _value )
{
	TTiXmlNode* node;
	for ( node = next; node; node = node->next )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}

const TTiXmlNode* TTiXmlNode::PreviousSibling( const char * _value ) const
{
	const TTiXmlNode* node;
	for ( node = prev; node; node = node->prev )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}

TTiXmlNode* TTiXmlNode::PreviousSibling( const char * _value )
{
	TTiXmlNode* node;
	for ( node = prev; node; node = node->prev )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}

void TTiXmlElement::RemoveAttribute( const char * name )
{
	TTiXml_STRING str( name );
	TTiXmlAttribute* node = attributeSet.Find( str );
	if ( node )
	{
		attributeSet.Remove( node );
		delete node;
	}
}

const TTiXmlElement* TTiXmlNode::FirstChildElement() const
{
	const TTiXmlNode* node;

	for (	node = FirstChild();
			node;
			node = node->NextSibling() )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}

TTiXmlElement* TTiXmlNode::FirstChildElement()
{
	TTiXmlNode* node;

	for (	node = FirstChild();
			node;
			node = node->NextSibling() )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}

const TTiXmlElement* TTiXmlNode::FirstChildElement( const char * _value ) const
{
	const TTiXmlNode* node;

	for (	node = FirstChild( _value );
			node;
			node = node->NextSibling( _value ) )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}

TTiXmlElement* TTiXmlNode::FirstChildElement( const char * _value )
{
	TTiXmlNode* node;

	for (	node = FirstChild( _value );
			node;
			node = node->NextSibling( _value ) )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}

const TTiXmlElement* TTiXmlNode::NextSiblingElement() const
{
	const TTiXmlNode* node;

	for (	node = NextSibling();
	node;
	node = node->NextSibling() )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}

TTiXmlElement* TTiXmlNode::NextSiblingElement()
{
	TTiXmlNode* node;

	for (	node = NextSibling();
	node;
	node = node->NextSibling() )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}

const TTiXmlElement* TTiXmlNode::NextSiblingElement( const char * _value ) const
{
	const TTiXmlNode* node;

	for (	node = NextSibling( _value );
	node;
	node = node->NextSibling( _value ) )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}

TTiXmlElement* TTiXmlNode::NextSiblingElement( const char * _value )
{
	TTiXmlNode* node;

	for (	node = NextSibling( _value );
	node;
	node = node->NextSibling( _value ) )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}


const TTiXmlDocument* TTiXmlNode::GetDocument() const
{
	const TTiXmlNode* node;

	for( node = this; node; node = node->parent )
	{
		if ( node->ToDocument() )
			return node->ToDocument();
	}
	return 0;
}

TTiXmlDocument* TTiXmlNode::GetDocument()
{
	TTiXmlNode* node;

	for( node = this; node; node = node->parent )
	{
		if ( node->ToDocument() )
			return node->ToDocument();
	}
	return 0;
}

TTiXmlElement::TTiXmlElement (const char * _value)
	: TTiXmlNode( TTiXmlNode::ELEMENT )
{
	firstChild = lastChild = 0;
	value = _value;
}


#ifdef TTiXml_USE_STL
TTiXmlElement::TTiXmlElement( const std::string& _value ) 
	: TTiXmlNode( TTiXmlNode::ELEMENT )
{
	firstChild = lastChild = 0;
	value = _value;
}
#endif


TTiXmlElement::TTiXmlElement( const TTiXmlElement& copy)
	: TTiXmlNode( TTiXmlNode::ELEMENT )
{
	firstChild = lastChild = 0;
	copy.CopyTo( this );	
}


void TTiXmlElement::operator=( const TTiXmlElement& base )
{
	ClearThis();
	base.CopyTo( this );
}


TTiXmlElement::~TTiXmlElement()
{
	ClearThis();
}


void TTiXmlElement::ClearThis()
{
	Clear();
	while( attributeSet.First() )
	{
		TTiXmlAttribute* node = attributeSet.First();
		attributeSet.Remove( node );
		delete node;
	}
}


const char * TTiXmlElement::Attribute( const char * name ) const
{
	TTiXml_STRING str( name );
	const TTiXmlAttribute* node = attributeSet.Find( str );

	if ( node )
		return node->Value();

	return 0;
}


const char * TTiXmlElement::Attribute( const char * name, int* i ) const
{
	const char * s = Attribute( name );
	if ( i )
	{
		if ( s )
			*i = atoi( s );
		else
			*i = 0;
	}
	return s;
}


const char * TTiXmlElement::Attribute( const char * name, double* d ) const
{
	const char * s = Attribute( name );
	if ( d )
	{
		if ( s )
			*d = atof( s );
		else
			*d = 0;
	}
	return s;
}


int TTiXmlElement::QueryIntAttribute( const char* name, int* ival ) const
{
	TTiXml_STRING str( name );
	const TTiXmlAttribute* node = attributeSet.Find( str );
	if ( !node )
		return TTiXml_NO_ATTRIBUTE;

	return node->QueryIntValue( ival );
}


int TTiXmlElement::QueryDoubleAttribute( const char* name, double* dval ) const
{
	TTiXml_STRING str( name );
	const TTiXmlAttribute* node = attributeSet.Find( str );
	if ( !node )
		return TTiXml_NO_ATTRIBUTE;

	return node->QueryDoubleValue( dval );
}


void TTiXmlElement::SetAttribute( const char * name, int val )
{	
	char buf[64];
	#if defined(TTiXml_SNPRINTF)		
		TTiXml_SNPRINTF( buf, sizeof(buf), "%d", val );
	#else
		sprintf( buf, "%d", val );
	#endif
	SetAttribute( name, buf );
}


#ifdef TTiXml_USE_STL
void TTiXmlElement::SetAttribute( const std::string& name, int val )
{	
   std::ostringstream oss;
   oss << val;
   SetAttribute( name, oss.str() );
}
#endif


void TTiXmlElement::SetDoubleAttribute( const char * name, double val )
{	
	char buf[256];
	#if defined(TTiXml_SNPRINTF)		
		TTiXml_SNPRINTF( buf, sizeof(buf), "%f", val );
	#else
		sprintf( buf, "%f", val );
	#endif
	SetAttribute( name, buf );
}


void TTiXmlElement::SetAttribute( const char * cname, const char * cvalue )
{
	TTiXml_STRING _name( cname );
	TTiXml_STRING _value( cvalue );

	TTiXmlAttribute* node = attributeSet.Find( _name );
	if ( node )
	{
		node->SetValue( cvalue );
		return;
	}

	TTiXmlAttribute* attrib = new TTiXmlAttribute( cname, cvalue );
	if ( attrib )
	{
		attributeSet.Add( attrib );
	}
	else
	{
		TTiXmlDocument* document = GetDocument();
		if ( document ) document->SetError( TTiXml_ERROR_OUT_OF_MEMORY, 0, 0, TTiXml_ENCODING_UNKNOWN );
	}
}


#ifdef TTiXml_USE_STL
void TTiXmlElement::SetAttribute( const std::string& name, const std::string& _value )
{
	TTiXmlAttribute* node = attributeSet.Find( name );
	if ( node )
	{
		node->SetValue( _value );
		return;
	}

	TTiXmlAttribute* attrib = new TTiXmlAttribute( name, _value );
	if ( attrib )
	{
		attributeSet.Add( attrib );
	}
	else
	{
		TTiXmlDocument* document = GetDocument();
		if ( document ) document->SetError( TTiXml_ERROR_OUT_OF_MEMORY, 0, 0, TTiXml_ENCODING_UNKNOWN );
	}
}
#endif


void TTiXmlElement::Print( FILE* cfile, int depth ) const
{
	int i;
	for ( i=0; i<depth; i++ )
	{
		fprintf( cfile, "    " );
	}

	fprintf( cfile, "<%s", value.c_str() );

	const TTiXmlAttribute* attrib;
	for ( attrib = attributeSet.First(); attrib; attrib = attrib->Next() )
	{
		fprintf( cfile, " " );
		attrib->Print( cfile, depth );
	}

	// There are 3 different formatting approaches:
	// 1) An element without children is printed as a <foo /> node
	// 2) An element with only a text child is printed as <foo> text </foo>
	// 3) An element with children is printed on multiple lines.
	TTiXmlNode* node;
	if ( !firstChild )
	{
		fprintf( cfile, " />" );
	}
	else if ( firstChild == lastChild && firstChild->ToText() )
	{
		fprintf( cfile, ">" );
		firstChild->Print( cfile, depth + 1 );
		fprintf( cfile, "</%s>", value.c_str() );
	}
	else
	{
		fprintf( cfile, ">" );

		for ( node = firstChild; node; node=node->NextSibling() )
		{
			if ( !node->ToText() )
			{
				fprintf( cfile, "\n" );
			}
			node->Print( cfile, depth+1 );
		}
		fprintf( cfile, "\n" );
		for( i=0; i<depth; ++i )
		fprintf( cfile, "    " );
		fprintf( cfile, "</%s>", value.c_str() );
	}
}

void TTiXmlElement::StreamOut( TTiXml_OSTREAM * stream ) const
{
	(*stream) << "<" << value;

	const TTiXmlAttribute* attrib;
	for ( attrib = attributeSet.First(); attrib; attrib = attrib->Next() )
	{	
		(*stream) << " ";
		attrib->StreamOut( stream );
	}

	// If this node has children, give it a closing tag. Else
	// make it an empty tag.
	TTiXmlNode* node;
	if ( firstChild )
	{ 		
		(*stream) << ">";

		for ( node = firstChild; node; node=node->NextSibling() )
		{
			node->StreamOut( stream );
		}
		(*stream) << "</" << value << ">";
	}
	else
	{
		(*stream) << " />";
	}
}


void TTiXmlElement::CopyTo( TTiXmlElement* target ) const
{
	// superclass:
	TTiXmlNode::CopyTo( target );

	// Element class: 
	// Clone the attributes, then clone the children.
	const TTiXmlAttribute* attribute = 0;
	for(	attribute = attributeSet.First();
	attribute;
	attribute = attribute->Next() )
	{
		target->SetAttribute( attribute->Name(), attribute->Value() );
	}

	TTiXmlNode* node = 0;
	for ( node = firstChild; node; node = node->NextSibling() )
	{
		target->LinkEndChild( node->Clone() );
	}
}


TTiXmlNode* TTiXmlElement::Clone() const
{
	TTiXmlElement* clone = new TTiXmlElement( Value() );
	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


const char* TTiXmlElement::GetText() const
{
	const TTiXmlNode* child = this->FirstChild();
	if ( child ) {
		const TTiXmlText* childText = child->ToText();
		if ( childText ) {
			return childText->Value();
		}
	}
	return 0;
}


TTiXmlDocument::TTiXmlDocument() : TTiXmlNode( TTiXmlNode::DOCUMENT )
{
	tabsize = 4;
	useMicrosoftBOM = false;
	ClearError();
}

TTiXmlDocument::TTiXmlDocument( const char * documentName ) : TTiXmlNode( TTiXmlNode::DOCUMENT )
{
	tabsize = 4;
	useMicrosoftBOM = false;
	value = documentName;
	ClearError();
}


#ifdef TTiXml_USE_STL
TTiXmlDocument::TTiXmlDocument( const std::string& documentName ) : TTiXmlNode( TTiXmlNode::DOCUMENT )
{
	tabsize = 4;
	useMicrosoftBOM = false;
    value = documentName;
	ClearError();
}
#endif


TTiXmlDocument::TTiXmlDocument( const TTiXmlDocument& copy ) : TTiXmlNode( TTiXmlNode::DOCUMENT )
{
	copy.CopyTo( this );
}


void TTiXmlDocument::operator=( const TTiXmlDocument& copy )
{
	Clear();
	copy.CopyTo( this );
}


bool TTiXmlDocument::LoadFile( TTiXmlEncoding encoding )
{
	// See STL_STRING_BUG below.
	StringToBuffer buf( value );

	if ( buf.buffer && LoadFile( buf.buffer, encoding ) )
		return true;

	return false;
}


bool TTiXmlDocument::SaveFile() const
{
	// See STL_STRING_BUG below.
	StringToBuffer buf( value );

	if ( buf.buffer && SaveFile( buf.buffer ) )
		return true;

	return false;
}

bool TTiXmlDocument::LoadFile( const char* filename, TTiXmlEncoding encoding )
{
	// There was a really terrifying little bug here. The code:
	//		value = filename
	// in the STL case, cause the assignment method of the std::string to
	// be called. What is strange, is that the std::string had the same
	// address as it's c_str() method, and so bad things happen. Looks
	// like a bug in the Microsoft STL implementation.
	// See STL_STRING_BUG above.
	// Fixed with the StringToBuffer class.
	value = filename;

	// reading in binary mode so that tinyxml can normalize the EOL
	FILE* file = fopen( value.c_str (), "rb" );	

	if ( file )
	{
		bool result = LoadFile( file, encoding );
		fclose( file );
		return result;
	}
	else
	{
		SetError( TTiXml_ERROR_OPENING_FILE, 0, 0, TTiXml_ENCODING_UNKNOWN );
		return false;
	}
}

bool TTiXmlDocument::LoadFile( FILE* file, TTiXmlEncoding encoding )
{
	if ( !file ) 
	{
		SetError( TTiXml_ERROR_OPENING_FILE, 0, 0, TTiXml_ENCODING_UNKNOWN );
		return false;
	}

	// Delete the existing data:
	Clear();
	location.Clear();

	// Get the file size, so we can pre-allocate the string. HUGE speed impact.
	long length = 0;
	fseek( file, 0, SEEK_END );
	length = ftell( file );
	fseek( file, 0, SEEK_SET );

	// Strange case, but good to handle up front.
	if ( length == 0 )
	{
		SetError( TTiXml_ERROR_DOCUMENT_EMPTY, 0, 0, TTiXml_ENCODING_UNKNOWN );
		return false;
	}

	// If we have a file, assume it is all one big XML file, and read it in.
	// The document parser may decide the document ends sooner than the entire file, however.
	TTiXml_STRING data;
	data.reserve( length );

	// Subtle bug here. TinyXml did use fgets. But from the XML spec:
	// 2.11 End-of-Line Handling
	// <snip>
	// <quote>
	// ...the XML processor MUST behave as if it normalized all line breaks in external 
	// parsed entities (including the document entity) on input, before parsing, by translating 
	// both the two-character sequence #xD #xA and any #xD that is not followed by #xA to 
	// a single #xA character.
	// </quote>
	//
	// It is not clear fgets does that, and certainly isn't clear it works cross platform. 
	// Generally, you expect fgets to translate from the convention of the OS to the c/unix
	// convention, and not work generally.

	/*
	while( fgets( buf, sizeof(buf), file ) )
	{
		data += buf;
	}
	*/

	char* buf = new char[ length+1 ];
	buf[0] = 0;

	if ( fread( buf, length, 1, file ) != 1 ) {
		SetError( TTiXml_ERROR_OPENING_FILE, 0, 0, TTiXml_ENCODING_UNKNOWN );
		return false;
	}

	const char* lastPos = buf;
	const char* p = buf;

	buf[length] = 0;
	while( *p ) {
		assert( p < (buf+length) );
		if ( *p == 0xa ) {
			// Newline character. No special rules for this. Append all the characters
			// since the last string, and include the newline.
			data.append( lastPos, (p-lastPos+1) );	// append, include the newline
			++p;									// move past the newline
			lastPos = p;							// and point to the new buffer (may be 0)
			assert( p <= (buf+length) );
		}
		else if ( *p == 0xd ) {
			// Carriage return. Append what we have so far, then
			// handle moving forward in the buffer.
			if ( (p-lastPos) > 0 ) {
				data.append( lastPos, p-lastPos );	// do not add the CR
			}
			data += (char)0xa;						// a proper newline

			if ( *(p+1) == 0xa ) {
				// Carriage return - new line sequence
				p += 2;
				lastPos = p;
				assert( p <= (buf+length) );
			}
			else {
				// it was followed by something else...that is presumably characters again.
				++p;
				lastPos = p;
				assert( p <= (buf+length) );
			}
		}
		else {
			++p;
		}
	}
	// Handle any left over characters.
	if ( p-lastPos ) {
		data.append( lastPos, p-lastPos );
	}		
	delete [] buf;
	buf = 0;

	Parse( data.c_str(), 0, encoding );

	if (  Error() )
        return false;
    else
		return true;
}


bool TTiXmlDocument::SaveFile( const char * filename ) const
{
	// The old c stuff lives on...
	FILE* fp = fopen( filename, "w" );
	if ( fp )
	{
		bool result = SaveFile( fp );
		fclose( fp );
		return result;
	}
	return false;
}


bool TTiXmlDocument::SaveFile( FILE* fp ) const
{
	if ( useMicrosoftBOM ) 
	{
		const unsigned char TTiXml_UTF_LEAD_0 = 0xefU;
		const unsigned char TTiXml_UTF_LEAD_1 = 0xbbU;
		const unsigned char TTiXml_UTF_LEAD_2 = 0xbfU;

		fputc( TTiXml_UTF_LEAD_0, fp );
		fputc( TTiXml_UTF_LEAD_1, fp );
		fputc( TTiXml_UTF_LEAD_2, fp );
	}
	Print( fp, 0 );
	return true;
}


void TTiXmlDocument::CopyTo( TTiXmlDocument* target ) const
{
	TTiXmlNode::CopyTo( target );

	target->error = error;
	target->errorDesc = errorDesc.c_str ();

	TTiXmlNode* node = 0;
	for ( node = firstChild; node; node = node->NextSibling() )
	{
		target->LinkEndChild( node->Clone() );
	}	
}


TTiXmlNode* TTiXmlDocument::Clone() const
{
	TTiXmlDocument* clone = new TTiXmlDocument();
	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


void TTiXmlDocument::Print( FILE* cfile, int depth ) const
{
	const TTiXmlNode* node;
	for ( node=FirstChild(); node; node=node->NextSibling() )
	{
		node->Print( cfile, depth );
		fprintf( cfile, "\n" );
	}
}

void TTiXmlDocument::StreamOut( TTiXml_OSTREAM * out ) const
{
	const TTiXmlNode* node;
	for ( node=FirstChild(); node; node=node->NextSibling() )
	{
		node->StreamOut( out );

		// Special rule for streams: stop after the root element.
		// The stream in code will only read one element, so don't
		// write more than one.
		if ( node->ToElement() )
			break;
	}
}


const TTiXmlAttribute* TTiXmlAttribute::Next() const
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( next->value.empty() && next->name.empty() )
		return 0;
	return next;
}

TTiXmlAttribute* TTiXmlAttribute::Next()
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( next->value.empty() && next->name.empty() )
		return 0;
	return next;
}

const TTiXmlAttribute* TTiXmlAttribute::Previous() const
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( prev->value.empty() && prev->name.empty() )
		return 0;
	return prev;
}

TTiXmlAttribute* TTiXmlAttribute::Previous()
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( prev->value.empty() && prev->name.empty() )
		return 0;
	return prev;
}

void TTiXmlAttribute::Print( FILE* cfile, int /*depth*/ ) const
{
	TTiXml_STRING n, v;

	PutString( name, &n );
	PutString( value, &v );

	if (value.find ('\"') == TTiXml_STRING::npos)
		fprintf (cfile, "%s=\"%s\"", n.c_str(), v.c_str() );
	else
		fprintf (cfile, "%s='%s'", n.c_str(), v.c_str() );
}


void TTiXmlAttribute::StreamOut( TTiXml_OSTREAM * stream ) const
{
	if (value.find( '\"' ) != TTiXml_STRING::npos)
	{
		PutString( name, stream );
		(*stream) << "=" << "'";
		PutString( value, stream );
		(*stream) << "'";
	}
	else
	{
		PutString( name, stream );
		(*stream) << "=" << "\"";
		PutString( value, stream );
		(*stream) << "\"";
	}
}

int TTiXmlAttribute::QueryIntValue( int* ival ) const
{
	if ( sscanf( value.c_str(), "%d", ival ) == 1 )
		return TTiXml_SUCCESS;
	return TTiXml_WRONG_TYPE;
}

int TTiXmlAttribute::QueryDoubleValue( double* dval ) const
{
	if ( sscanf( value.c_str(), "%lf", dval ) == 1 )
		return TTiXml_SUCCESS;
	return TTiXml_WRONG_TYPE;
}

void TTiXmlAttribute::SetIntValue( int _value )
{
	char buf [64];
	#if defined(TTiXml_SNPRINTF)		
		TTiXml_SNPRINTF(buf, sizeof(buf), "%d", _value);
	#else
		sprintf (buf, "%d", _value);
	#endif
	SetValue (buf);
}

void TTiXmlAttribute::SetDoubleValue( double _value )
{
	char buf [256];
	#if defined(TTiXml_SNPRINTF)		
		TTiXml_SNPRINTF( buf, sizeof(buf), "%lf", _value);
	#else
		sprintf (buf, "%lf", _value);
	#endif
	SetValue (buf);
}

int TTiXmlAttribute::IntValue() const
{
	return atoi (value.c_str ());
}

double  TTiXmlAttribute::DoubleValue() const
{
	return atof (value.c_str ());
}


TTiXmlComment::TTiXmlComment( const TTiXmlComment& copy ) : TTiXmlNode( TTiXmlNode::COMMENT )
{
	copy.CopyTo( this );
}


void TTiXmlComment::operator=( const TTiXmlComment& base )
{
	Clear();
	base.CopyTo( this );
}


void TTiXmlComment::Print( FILE* cfile, int depth ) const
{
	for ( int i=0; i<depth; i++ )
	{
		fputs( "    ", cfile );
	}
	fprintf( cfile, "<!--%s-->", value.c_str() );
}

void TTiXmlComment::StreamOut( TTiXml_OSTREAM * stream ) const
{
	(*stream) << "<!--";
	//PutString( value, stream );
	(*stream) << value;
	(*stream) << "-->";
}


void TTiXmlComment::CopyTo( TTiXmlComment* target ) const
{
	TTiXmlNode::CopyTo( target );
}


TTiXmlNode* TTiXmlComment::Clone() const
{
	TTiXmlComment* clone = new TTiXmlComment();

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


void TTiXmlText::Print( FILE* cfile, int depth ) const
{
	if ( cdata )
	{
		int i;
		fprintf( cfile, "\n" );
		for ( i=0; i<depth; i++ ) {
			fprintf( cfile, "    " );
		}
		fprintf( cfile, "<![CDATA[" );
		fprintf( cfile, "%s", value.c_str() );	// unformatted output
		fprintf( cfile, "]]>\n" );
	}
	else
	{
		TTiXml_STRING buffer;
		PutString( value, &buffer );
		fprintf( cfile, "%s", buffer.c_str() );
	}
}


void TTiXmlText::StreamOut( TTiXml_OSTREAM * stream ) const
{
	if ( cdata )
	{
		(*stream) << "<![CDATA[" << value << "]]>";
	}
	else
	{
		PutString( value, stream );
	}
}


void TTiXmlText::CopyTo( TTiXmlText* target ) const
{
	TTiXmlNode::CopyTo( target );
	target->cdata = cdata;
}


TTiXmlNode* TTiXmlText::Clone() const
{	
	TTiXmlText* clone = 0;
	clone = new TTiXmlText( "" );

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


TTiXmlDeclaration::TTiXmlDeclaration( const char * _version,
									const char * _encoding,
									const char * _standalone )
	: TTiXmlNode( TTiXmlNode::DECLARATION )
{
	version = _version;
	encoding = _encoding;
	standalone = _standalone;
}


#ifdef TTiXml_USE_STL
TTiXmlDeclaration::TTiXmlDeclaration(	const std::string& _version,
									const std::string& _encoding,
									const std::string& _standalone )
	: TTiXmlNode( TTiXmlNode::DECLARATION )
{
	version = _version;
	encoding = _encoding;
	standalone = _standalone;
}
#endif


TTiXmlDeclaration::TTiXmlDeclaration( const TTiXmlDeclaration& copy )
	: TTiXmlNode( TTiXmlNode::DECLARATION )
{
	copy.CopyTo( this );	
}


void TTiXmlDeclaration::operator=( const TTiXmlDeclaration& copy )
{
	Clear();
	copy.CopyTo( this );
}


void TTiXmlDeclaration::Print( FILE* cfile, int /*depth*/ ) const
{
	fprintf (cfile, "<?xml ");

	if ( !version.empty() )
		fprintf (cfile, "version=\"%s\" ", version.c_str ());
	if ( !encoding.empty() )
		fprintf (cfile, "encoding=\"%s\" ", encoding.c_str ());
	if ( !standalone.empty() )
		fprintf (cfile, "standalone=\"%s\" ", standalone.c_str ());
	fprintf (cfile, "?>");
}

void TTiXmlDeclaration::StreamOut( TTiXml_OSTREAM * stream ) const
{
	(*stream) << "<?xml ";

	if ( !version.empty() )
	{
		(*stream) << "version=\"";
		PutString( version, stream );
		(*stream) << "\" ";
	}
	if ( !encoding.empty() )
	{
		(*stream) << "encoding=\"";
		PutString( encoding, stream );
		(*stream ) << "\" ";
	}
	if ( !standalone.empty() )
	{
		(*stream) << "standalone=\"";
		PutString( standalone, stream );
		(*stream) << "\" ";
	}
	(*stream) << "?>";
}


void TTiXmlDeclaration::CopyTo( TTiXmlDeclaration* target ) const
{
	TTiXmlNode::CopyTo( target );

	target->version = version;
	target->encoding = encoding;
	target->standalone = standalone;
}


TTiXmlNode* TTiXmlDeclaration::Clone() const
{	
	TTiXmlDeclaration* clone = new TTiXmlDeclaration();

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


void TTiXmlUnknown::Print( FILE* cfile, int depth ) const
{
	for ( int i=0; i<depth; i++ )
		fprintf( cfile, "    " );
	fprintf( cfile, "<%s>", value.c_str() );
}


void TTiXmlUnknown::StreamOut( TTiXml_OSTREAM * stream ) const
{
	(*stream) << "<" << value << ">";		// Don't use entities here! It is unknown.
}


void TTiXmlUnknown::CopyTo( TTiXmlUnknown* target ) const
{
	TTiXmlNode::CopyTo( target );
}


TTiXmlNode* TTiXmlUnknown::Clone() const
{
	TTiXmlUnknown* clone = new TTiXmlUnknown();

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


TTiXmlAttributeSet::TTiXmlAttributeSet()
{
	sentinel.next = &sentinel;
	sentinel.prev = &sentinel;
}


TTiXmlAttributeSet::~TTiXmlAttributeSet()
{
	assert( sentinel.next == &sentinel );
	assert( sentinel.prev == &sentinel );
}


void TTiXmlAttributeSet::Add( TTiXmlAttribute* addMe )
{
	assert( !Find( TTiXml_STRING( addMe->Name() ) ) );	// Shouldn't be multiply adding to the set.

	addMe->next = &sentinel;
	addMe->prev = sentinel.prev;

	sentinel.prev->next = addMe;
	sentinel.prev      = addMe;
}

void TTiXmlAttributeSet::Remove( TTiXmlAttribute* removeMe )
{
	TTiXmlAttribute* node;

	for( node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( node == removeMe )
		{
			node->prev->next = node->next;
			node->next->prev = node->prev;
			node->next = 0;
			node->prev = 0;
			return;
		}
	}
	assert( 0 );		// we tried to remove a non-linked attribute.
}

const TTiXmlAttribute* TTiXmlAttributeSet::Find( const TTiXml_STRING& name ) const
{
	const TTiXmlAttribute* node;

	for( node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( node->name == name )
			return node;
	}
	return 0;
}

TTiXmlAttribute*	TTiXmlAttributeSet::Find( const TTiXml_STRING& name )
{
	TTiXmlAttribute* node;

	for( node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( node->name == name )
			return node;
	}
	return 0;
}

#ifdef TTiXml_USE_STL	
TTiXml_ISTREAM & operator >> (TTiXml_ISTREAM & in, TTiXmlNode & base)
{
	TTiXml_STRING tag;
	tag.reserve( 8 * 1000 );
	base.StreamIn( &in, &tag );

	base.Parse( tag.c_str(), 0, TTiXml_DEFAULT_ENCODING );
	return in;
}
#endif


TTiXml_OSTREAM & operator<< (TTiXml_OSTREAM & out, const TTiXmlNode & base)
{
	base.StreamOut (& out);
	return out;
}


#ifdef TTiXml_USE_STL	
std::string & operator<< (std::string& out, const TTiXmlNode& base )
{
   std::ostringstream os_stream( std::ostringstream::out );
   base.StreamOut( &os_stream );
   
   out.append( os_stream.str() );
   return out;
}
#endif


TTiXmlHandle TTiXmlHandle::FirstChild() const
{
	if ( node )
	{
		TTiXmlNode* child = node->FirstChild();
		if ( child )
			return TTiXmlHandle( child );
	}
	return TTiXmlHandle( 0 );
}


TTiXmlHandle TTiXmlHandle::FirstChild( const char * value ) const
{
	if ( node )
	{
		TTiXmlNode* child = node->FirstChild( value );
		if ( child )
			return TTiXmlHandle( child );
	}
	return TTiXmlHandle( 0 );
}


TTiXmlHandle TTiXmlHandle::FirstChildElement() const
{
	if ( node )
	{
		TTiXmlElement* child = node->FirstChildElement();
		if ( child )
			return TTiXmlHandle( child );
	}
	return TTiXmlHandle( 0 );
}


TTiXmlHandle TTiXmlHandle::FirstChildElement( const char * value ) const
{
	if ( node )
	{
		TTiXmlElement* child = node->FirstChildElement( value );
		if ( child )
			return TTiXmlHandle( child );
	}
	return TTiXmlHandle( 0 );
}


TTiXmlHandle TTiXmlHandle::Child( int count ) const
{
	if ( node )
	{
		int i;
		TTiXmlNode* child = node->FirstChild();
		for (	i=0;
				child && i<count;
				child = child->NextSibling(), ++i )
		{
			// nothing
		}
		if ( child )
			return TTiXmlHandle( child );
	}
	return TTiXmlHandle( 0 );
}


TTiXmlHandle TTiXmlHandle::Child( const char* value, int count ) const
{
	if ( node )
	{
		int i;
		TTiXmlNode* child = node->FirstChild( value );
		for (	i=0;
				child && i<count;
				child = child->NextSibling( value ), ++i )
		{
			// nothing
		}
		if ( child )
			return TTiXmlHandle( child );
	}
	return TTiXmlHandle( 0 );
}


TTiXmlHandle TTiXmlHandle::ChildElement( int count ) const
{
	if ( node )
	{
		int i;
		TTiXmlElement* child = node->FirstChildElement();
		for (	i=0;
				child && i<count;
				child = child->NextSiblingElement(), ++i )
		{
			// nothing
		}
		if ( child )
			return TTiXmlHandle( child );
	}
	return TTiXmlHandle( 0 );
}


TTiXmlHandle TTiXmlHandle::ChildElement( const char* value, int count ) const
{
	if ( node )
	{
		int i;
		TTiXmlElement* child = node->FirstChildElement( value );
		for (	i=0;
				child && i<count;
				child = child->NextSiblingElement( value ), ++i )
		{
			// nothing
		}
		if ( child )
			return TTiXmlHandle( child );
	}
	return TTiXmlHandle( 0 );
}
