import React from "react";

import {NavItem} from "components/NavItem";

import "css/main.scss";

class MainTemplate extends React.Component {
	static propTypes() {
		const propTypes = {
			children: React.PropTypes.object,
			location: React.PropTypes.object.isRequired
		};
		return propTypes;
	}
	render() {
		return (
			<div>
				<ul className="nav nav-tabs">
					<div>
						<NavItem location={this.props.location} link="/docs/use/">
							Use
						</NavItem>
						<NavItem location={this.props.location} link="/docs/config/">
							Configure
						</NavItem>
						<NavItem location={this.props.location} link="/docs/opts/">
							Command-line options
						</NavItem>
						<NavItem location={this.props.location} link="/docs/faq/">
							FAQ
						</NavItem>
					</div>
				</ul>
				<main>
					{this.props.children}
				</main>
			</div>
		);
	}
}

export default MainTemplate;
