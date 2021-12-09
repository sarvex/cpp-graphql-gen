// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#include "DirectiveObject.h"
#include "InputValueObject.h"

#include "graphqlservice/internal/Introspection.h"

#include <algorithm>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

using namespace std::literals;

namespace graphql::introspection {
namespace object {

Directive::Directive(std::shared_ptr<introspection::Directive> pimpl) noexcept
	: service::Object{ getTypeNames(), getResolvers() }
	, _pimpl { std::make_unique<Model<introspection::Directive>>(std::move(pimpl)) }
{
}

Directive::~Directive()
{
	// This is empty, but explicitly defined here so that it can access the un-exported destructor
	// of the implementation type.
}

service::TypeNames Directive::getTypeNames() const noexcept
{
	return {
		R"gql(__Directive)gql"sv
	};
}

service::ResolverMap Directive::getResolvers() const noexcept
{
	return {
		{ R"gql(args)gql"sv, [this](service::ResolverParams&& params) { return resolveArgs(std::move(params)); } },
		{ R"gql(name)gql"sv, [this](service::ResolverParams&& params) { return resolveName(std::move(params)); } },
		{ R"gql(locations)gql"sv, [this](service::ResolverParams&& params) { return resolveLocations(std::move(params)); } },
		{ R"gql(__typename)gql"sv, [this](service::ResolverParams&& params) { return resolve_typename(std::move(params)); } },
		{ R"gql(description)gql"sv, [this](service::ResolverParams&& params) { return resolveDescription(std::move(params)); } }
	};
}

service::AwaitableResolver Directive::resolveName(service::ResolverParams&& params) const
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getName(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<std::string>::convert(std::move(result), std::move(params));
}

service::AwaitableResolver Directive::resolveDescription(service::ResolverParams&& params) const
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getDescription(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<std::string>::convert<service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

service::AwaitableResolver Directive::resolveLocations(service::ResolverParams&& params) const
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getLocations(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<DirectiveLocation>::convert<service::TypeModifier::List>(std::move(result), std::move(params));
}

service::AwaitableResolver Directive::resolveArgs(service::ResolverParams&& params) const
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getArgs(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<InputValue>::convert<service::TypeModifier::List>(std::move(result), std::move(params));
}

service::AwaitableResolver Directive::resolve_typename(service::ResolverParams&& params) const
{
	return service::ModifiedResult<std::string>::convert(std::string{ R"gql(__Directive)gql" }, std::move(params));
}

} // namespace object

void AddDirectiveDetails(const std::shared_ptr<schema::ObjectType>& typeDirective, const std::shared_ptr<schema::Schema>& schema)
{
	typeDirective->AddFields({
		schema::Field::Make(R"gql(name)gql"sv, R"md()md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(String)gql"sv))),
		schema::Field::Make(R"gql(description)gql"sv, R"md()md"sv, std::nullopt, schema->LookupType(R"gql(String)gql"sv)),
		schema::Field::Make(R"gql(locations)gql"sv, R"md()md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(__DirectiveLocation)gql"sv))))),
		schema::Field::Make(R"gql(args)gql"sv, R"md()md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(__InputValue)gql"sv)))))
	});
}

} // namespace graphql::introspection
