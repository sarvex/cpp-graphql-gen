// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef FOLDEREDGEOBJECT_H
#define FOLDEREDGEOBJECT_H

#include "TodaySchema.h"

namespace graphql::today::object {

class FolderEdge
	: public service::Object
{
private:
	service::AwaitableResolver resolveNode(service::ResolverParams&& params);
	service::AwaitableResolver resolveCursor(service::ResolverParams&& params);

	service::AwaitableResolver resolve_typename(service::ResolverParams&& params);

	struct Concept
	{
		virtual service::FieldResult<std::shared_ptr<Folder>> getNode(service::FieldParams&& params) const = 0;
		virtual service::FieldResult<response::Value> getCursor(service::FieldParams&& params) const = 0;
	};

	template <class T>
	struct Model
		: Concept
	{
		Model(std::shared_ptr<T>&& pimpl) noexcept
			: _pimpl { std::move(pimpl) }
		{
		}

		service::FieldResult<std::shared_ptr<Folder>> getNode(service::FieldParams&& params) const final
		{
			return _pimpl->getNode(std::move(params));
		}

		service::FieldResult<response::Value> getCursor(service::FieldParams&& params) const final
		{
			return _pimpl->getCursor(std::move(params));
		}

	private:
		const std::shared_ptr<T> _pimpl;
	};

	FolderEdge(std::unique_ptr<Concept>&& pimpl);

	const std::unique_ptr<Concept> _pimpl;

public:
	template <class T>
	FolderEdge(std::shared_ptr<T> pimpl)
		: FolderEdge { std::make_unique<Model<T>>(std::move(pimpl)) }
	{
	}

	~FolderEdge();
};

} // namespace graphql::today::object

#endif // FOLDEREDGEOBJECT_H
