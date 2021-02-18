#ifndef AXIOM_XML_HPP
#define AXIOM_XML_HPP

#include <sstream>
#include <list>

namespace Axiom
{

	namespace XML
	{
		class Attribute
		{
		public:
			Attribute(const std::string& name, const std::string& value)
				: name_(name), value_(value)
			{
			}

			const std::string& GetName() const { return name_; }
			const std::string& GetValue() const { return value_; }

		protected:
			std::string name_;
			std::string value_;
		};

		class Element
		{
		public:
			Element(const std::string& name, const std::list<Attribute>& attributes, const std::string& content)
				: name_(name), attributes_(attributes)
			{
			}

			const std::string& GetName() const { return name_; }
			const std::string& Content() const { return content_; }
			std::string& Content() { return content_; }

			std::list<Attribute>& Attributes() { return attributes_; }
			const std::list<Attribute>& Attributes() const { return attributes_; }

			std::list<Element>& Elements() { return elements_; }
			const std::list<Element>& Elements() const { return elements_; }

		protected:
			std::string name_;
			std::string content_;
			std::list<Attribute> attributes_;
			std::list<Element> elements_;
		};
		
		static void ReplaceAll(std::string& str, const std::string& what, const std::string& with)
		{
			std::size_t itr = str.find(what);
			while (itr != std::string::npos)
			{
				str.replace(itr, itr + what.size(), with.c_str());
				itr = str.find(what);
			}
		}

		static std::string EscapeXMLChars(const std::string& syntaxToConvert)
		{
			std::string convertedSyntax = syntaxToConvert;

			// search for an escape char...
			ReplaceAll(convertedSyntax, ">", "&gt;");
			ReplaceAll(convertedSyntax, "<", "&lt;");
			ReplaceAll(convertedSyntax, "&", "&amp;");
			ReplaceAll(convertedSyntax, "'", "&apos;");
			ReplaceAll(convertedSyntax, "\"", "&quot;");

			return convertedSyntax;
		}

		class Indent
		{
		public:

			Indent(const std::string& indentation)
				: indentation_(indentation)
			{
			}

			Indent(const Indent& indent, const std::string& indentation)
			{
				indentation_ = indent.GetIndentationString();
				indentation_.append(indentation);
			}

			const std::string& GetIndentationString() const
			{
				return indentation_;
			}

		private:
			std::string indentation_;
		};

		static std::string OutputElementAndAttributes(const Element& element, const Indent& currentIndent)
		{
			std::ostringstream oss;

			// Output element line (with attributes)
			oss << currentIndent.GetIndentationString() << "<" << element.GetName() << " ";
			for (auto itr = element.Attributes().begin(); itr != element.Attributes().end(); ++itr)
				oss << itr->GetName() << "=\"" << EscapeXMLChars(itr->GetValue()) << "\" ";

			// If no content in this element, inline it...
			if (element.Content().empty() && element.Elements().empty())
				oss << "/>\n";

			// Otherwise...
			else
			{
				oss << ">\n";
				
				if (!element.Content().empty())
					oss << currentIndent.GetIndentationString() << EscapeXMLChars(element.Content());

				// Output all child elements...
				for (auto itr = element.Elements().begin(); itr != element.Elements().end(); ++itr)
					oss << OutputElementAndAttributes(*itr, Indent(currentIndent, std::string("  ")));

				// Close the element
				oss << currentIndent.GetIndentationString() << "</" << element.GetName() << ">\n";
			}

			return oss.str();
		}

		static std::string GenerateSyntax(const Element& root)
		{
			std::ostringstream oss;
			oss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
			oss << OutputElementAndAttributes(root, Indent(std::string()));
			return oss.str();
		}

	}

}

#endif //AXIOM_XML_HPP
