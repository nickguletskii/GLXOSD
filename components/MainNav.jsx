import React from "react";
import {NavItem} from "components/NavItem";

export class MainNav extends React.Component {
	static propTypes() {
		const propTypes = {
			link: React.PropTypes.string.isRequired
		};
		return propTypes;
	}
	render() {
		return (
			<ul className="nav navbar-nav pull-xs-right">

				<NavItem location={this.props.location} link="/" exact>
					Home
				</NavItem>

				<NavItem location={this.props.location} link="/install/" exact>
					Install
				</NavItem>

				<NavItem location={this.props.location} link="/docs/">
					Docs
				</NavItem>

				<NavItem location={this.props.location} link="/analyse/" exact>
					Analyse
				</NavItem>

				<li className="nav-item">
					<a className="nav-link" href="https://github.com/nickguletskii/GLXOSD/">Source</a>
				</li>
				<li className="nav-item">
					<a className="nav-link" href="https://github.com/nickguletskii/GLXOSD/issues">Issue tracker</a>
				</li>
			</ul>
		);
	}
}
