import React from "react";
import {Link} from "react-router";
import {prefixLink} from "gatsby-helpers";
import includes from "lodash/includes";

import classNames from "classnames";

export class NavItem extends React.Component {
	static propTypes() {
		const propTypes = {
			children: React.PropTypes.object.isRequired,
			link: React.PropTypes.string.isRequired,
			exact: React.PropTypes.boolean
		};
		return propTypes;
	}
	get navLinkIsActive() {
		let isActive = false;
		if (this.props.exact) {
			isActive = this.props.location.pathname === this.props.link;
		} else {
			isActive = includes(this.props.location.pathname, this.props.link);
		}
		return classNames("nav-link", {active: isActive});
	}
	render() {
		return (
			<li className="nav-item">
				<Link className={this.navLinkIsActive} to={prefixLink(this.props.link)}>
					{this.props.children}
				</Link>
			</li>
		);
	}
}
