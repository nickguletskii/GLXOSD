const webpack = require("webpack");
exports.modifyWebpackConfig = function(config, env) {
	config.plugin("define-static-env", webpack.DefinePlugin, [{
		"process.env": {
			STATIC_ENV: (env === "build-html" || env === "build-css" || env == "develop-html")
		}
	}]);
	return config;
};
