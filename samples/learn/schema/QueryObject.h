// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef QUERYOBJECT_H
#define QUERYOBJECT_H

#include "StarWarsSchema.h"

namespace graphql::learn::object {
namespace methods::QueryHas {

template <class TImpl>
concept getHeroWithParams = requires (TImpl impl, service::FieldParams params, std::optional<Episode> episodeArg) 
{
	{ service::FieldResult<std::shared_ptr<Character>> { impl.getHero(std::move(params), std::move(episodeArg)) } };
};

template <class TImpl>
concept getHero = requires (TImpl impl, std::optional<Episode> episodeArg) 
{
	{ service::FieldResult<std::shared_ptr<Character>> { impl.getHero(std::move(episodeArg)) } };
};

template <class TImpl>
concept getHumanWithParams = requires (TImpl impl, service::FieldParams params, std::string idArg) 
{
	{ service::FieldResult<std::shared_ptr<Human>> { impl.getHuman(std::move(params), std::move(idArg)) } };
};

template <class TImpl>
concept getHuman = requires (TImpl impl, std::string idArg) 
{
	{ service::FieldResult<std::shared_ptr<Human>> { impl.getHuman(std::move(idArg)) } };
};

template <class TImpl>
concept getDroidWithParams = requires (TImpl impl, service::FieldParams params, std::string idArg) 
{
	{ service::FieldResult<std::shared_ptr<Droid>> { impl.getDroid(std::move(params), std::move(idArg)) } };
};

template <class TImpl>
concept getDroid = requires (TImpl impl, std::string idArg) 
{
	{ service::FieldResult<std::shared_ptr<Droid>> { impl.getDroid(std::move(idArg)) } };
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

} // namespace methods::QueryHas

class Query
	: public service::Object
{
private:
	service::AwaitableResolver resolveHero(service::ResolverParams&& params) const;
	service::AwaitableResolver resolveHuman(service::ResolverParams&& params) const;
	service::AwaitableResolver resolveDroid(service::ResolverParams&& params) const;

	service::AwaitableResolver resolve_typename(service::ResolverParams&& params) const;
	service::AwaitableResolver resolve_schema(service::ResolverParams&& params) const;
	service::AwaitableResolver resolve_type(service::ResolverParams&& params) const;

	std::shared_ptr<schema::Schema> _schema;

	struct Concept
	{
		virtual ~Concept() = default;

		virtual void beginSelectionSet(const service::SelectionSetParams& params) const = 0;
		virtual void endSelectionSet(const service::SelectionSetParams& params) const = 0;

		virtual service::FieldResult<std::shared_ptr<Character>> getHero(service::FieldParams&& params, std::optional<Episode>&& episodeArg) const = 0;
		virtual service::FieldResult<std::shared_ptr<Human>> getHuman(service::FieldParams&& params, std::string&& idArg) const = 0;
		virtual service::FieldResult<std::shared_ptr<Droid>> getDroid(service::FieldParams&& params, std::string&& idArg) const = 0;
	};

	template <class T>
	struct Model
		: Concept
	{
		Model(std::shared_ptr<T>&& pimpl) noexcept
			: _pimpl { std::move(pimpl) }
		{
		}

		service::FieldResult<std::shared_ptr<Character>> getHero(service::FieldParams&& params, std::optional<Episode>&& episodeArg) const final
		{
			if constexpr (methods::QueryHas::getHeroWithParams<T>)
			{
				return { _pimpl->getHero(std::move(params), std::move(episodeArg)) };
			}
			else
			{
				static_assert(methods::QueryHas::getHero<T>, R"msg(Query::getHero is not implemented)msg");
				return { _pimpl->getHero(std::move(episodeArg)) };
			}
		}

		service::FieldResult<std::shared_ptr<Human>> getHuman(service::FieldParams&& params, std::string&& idArg) const final
		{
			if constexpr (methods::QueryHas::getHumanWithParams<T>)
			{
				return { _pimpl->getHuman(std::move(params), std::move(idArg)) };
			}
			else
			{
				static_assert(methods::QueryHas::getHuman<T>, R"msg(Query::getHuman is not implemented)msg");
				return { _pimpl->getHuman(std::move(idArg)) };
			}
		}

		service::FieldResult<std::shared_ptr<Droid>> getDroid(service::FieldParams&& params, std::string&& idArg) const final
		{
			if constexpr (methods::QueryHas::getDroidWithParams<T>)
			{
				return { _pimpl->getDroid(std::move(params), std::move(idArg)) };
			}
			else
			{
				static_assert(methods::QueryHas::getDroid<T>, R"msg(Query::getDroid is not implemented)msg");
				return { _pimpl->getDroid(std::move(idArg)) };
			}
		}

		void beginSelectionSet(const service::SelectionSetParams& params) const final
		{
			if constexpr (methods::QueryHas::beginSelectionSet<T>)
			{
				_pimpl->beginSelectionSet(params);
			}
		}

		void endSelectionSet(const service::SelectionSetParams& params) const final
		{
			if constexpr (methods::QueryHas::endSelectionSet<T>)
			{
				_pimpl->endSelectionSet(params);
			}
		}

	private:
		const std::shared_ptr<T> _pimpl;
	};

	Query(std::unique_ptr<Concept>&& pimpl) noexcept;

	service::TypeNames getTypeNames() const noexcept;
	service::ResolverMap getResolvers() const noexcept;

	void beginSelectionSet(const service::SelectionSetParams& params) const final;
	void endSelectionSet(const service::SelectionSetParams& params) const final;

	const std::unique_ptr<Concept> _pimpl;

public:
	template <class T>
	Query(std::shared_ptr<T> pimpl) noexcept
		: Query { std::unique_ptr<Concept> { std::make_unique<Model<T>>(std::move(pimpl)) } }
	{
	}
};

} // namespace graphql::learn::object

#endif // QUERYOBJECT_H
