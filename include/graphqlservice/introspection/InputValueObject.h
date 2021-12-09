// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef INPUTVALUEOBJECT_H
#define INPUTVALUEOBJECT_H

#include "IntrospectionSchema.h"

namespace graphql::introspection::object {
namespace methods::InputValueHas {

template <class TImpl>
concept getNameWithParams = requires (TImpl impl, service::FieldParams params) 
{
	{ service::FieldResult<std::string> { impl.getName(std::move(params)) } };
};

template <class TImpl>
concept getName = requires (TImpl impl) 
{
	{ service::FieldResult<std::string> { impl.getName() } };
};

template <class TImpl>
concept getDescriptionWithParams = requires (TImpl impl, service::FieldParams params) 
{
	{ service::FieldResult<std::optional<std::string>> { impl.getDescription(std::move(params)) } };
};

template <class TImpl>
concept getDescription = requires (TImpl impl) 
{
	{ service::FieldResult<std::optional<std::string>> { impl.getDescription() } };
};

template <class TImpl>
concept getTypeWithParams = requires (TImpl impl, service::FieldParams params) 
{
	{ service::FieldResult<std::shared_ptr<Type>> { impl.getType(std::move(params)) } };
};

template <class TImpl>
concept getType = requires (TImpl impl) 
{
	{ service::FieldResult<std::shared_ptr<Type>> { impl.getType() } };
};

template <class TImpl>
concept getDefaultValueWithParams = requires (TImpl impl, service::FieldParams params) 
{
	{ service::FieldResult<std::optional<std::string>> { impl.getDefaultValue(std::move(params)) } };
};

template <class TImpl>
concept getDefaultValue = requires (TImpl impl) 
{
	{ service::FieldResult<std::optional<std::string>> { impl.getDefaultValue() } };
};

template <class TImpl>
concept beginSelectionSet = requires (TImpl impl, const service::SelectionSetParams params) 
{
	{ impl.beginSelectionSet(params) };
};

template <class TImpl>
concept endSelectionSet = requires (TImpl impl, const service::SelectionSetParams params) 
{
	{ impl.endSelectionSet(params) };
};

} // namespace methods::InputValueHas

class InputValue
	: public service::Object
{
private:
	service::AwaitableResolver resolveName(service::ResolverParams&& params) const;
	service::AwaitableResolver resolveDescription(service::ResolverParams&& params) const;
	service::AwaitableResolver resolveType(service::ResolverParams&& params) const;
	service::AwaitableResolver resolveDefaultValue(service::ResolverParams&& params) const;

	service::AwaitableResolver resolve_typename(service::ResolverParams&& params) const;

	struct Concept
	{
		virtual ~Concept() = default;

		virtual service::FieldResult<std::string> getName(service::FieldParams&& params) const = 0;
		virtual service::FieldResult<std::optional<std::string>> getDescription(service::FieldParams&& params) const = 0;
		virtual service::FieldResult<std::shared_ptr<Type>> getType(service::FieldParams&& params) const = 0;
		virtual service::FieldResult<std::optional<std::string>> getDefaultValue(service::FieldParams&& params) const = 0;
	};

	template <class T>
	struct Model
		: Concept
	{
		Model(std::shared_ptr<T>&& pimpl) noexcept
			: _pimpl { std::move(pimpl) }
		{
		}

		service::FieldResult<std::string> getName(service::FieldParams&&) const final
		{
			return { _pimpl->getName() };
		}

		service::FieldResult<std::optional<std::string>> getDescription(service::FieldParams&&) const final
		{
			return { _pimpl->getDescription() };
		}

		service::FieldResult<std::shared_ptr<Type>> getType(service::FieldParams&&) const final
		{
			return { _pimpl->getType() };
		}

		service::FieldResult<std::optional<std::string>> getDefaultValue(service::FieldParams&&) const final
		{
			return { _pimpl->getDefaultValue() };
		}

	private:
		const std::shared_ptr<T> _pimpl;
	};

	const std::unique_ptr<Concept> _pimpl;

	service::TypeNames getTypeNames() const noexcept;
	service::ResolverMap getResolvers() const noexcept;

public:
	GRAPHQLINTROSPECTION_EXPORT InputValue(std::shared_ptr<introspection::InputValue> pimpl) noexcept;
	GRAPHQLINTROSPECTION_EXPORT ~InputValue();
};

} // namespace graphql::introspection::object

#endif // INPUTVALUEOBJECT_H
