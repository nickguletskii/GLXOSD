import ReactGA from 'react-ga'

const webpack = require("webpack");
exports.modifyWebpackConfig = function(config, env) {
	config.plugin("define-static-env", webpack.DefinePlugin, [{
		"process.env": {
			STATIC_ENV: (env === "static")
		}
	}]);
	return config;
};

ReactGA.initialize("UA-46764899-1");
exports.onRouteUpdate = function(state, page, pages) {
	ReactGA.pageview(state.pathname + (state.hash ? state.hash : ""));
};
