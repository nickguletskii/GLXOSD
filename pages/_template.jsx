import React from "react";

import {MainNav} from "components/MainNav";
import Footer from "components/Footer";
import {Link} from "react-router";
import {prefixLink} from "gatsby-helpers";

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
				<nav
					className="navbar navbar-light bg-faded navbar-dark bg-primary">
					<Link className="navbar-brand" to={prefixLink("/")}>
						GLXOSD
					</Link>
					<MainNav location={this.props.location}/>
				</nav>

				<main>
					{this.props.children}
				</main>

				<Footer/>
			</div>
		);
	}
}

export default MainTemplate;
