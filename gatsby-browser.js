const webpack = require("webpack");
exports.modifyWebpackConfig = function(config, env) {
	config.plugin("define-static-env", webpack.DefinePlugin, [{
		"process.env": {
			STATIC_ENV: JSON.stringify(env==="static")
		}
	}]);
	return config;
};

exports.onRouteChange = function(state, page, pages) {
	require("analytics/index").pageview(state.pathname + (state.hash ? state.hash : ""));
};
