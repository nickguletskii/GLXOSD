define('modernizr', [], Modernizr);

require(['jquery', 'foundation'], function ($, foundation) {
	$(document)
		.foundation();
});
define(function (require) {

	var module,
		gaName = "ga"; // Global name of analytics object. Defaults to `ga`.

	// Setup temporary Google Analytics objects.
	window.GoogleAnalyticsObject = gaName;
	window[gaName] = function () {
		(window[gaName].q = window[gaName].q || [])
			.push(arguments);
	};
	window[gaName].l = 1 * new Date();

	// Immediately add a pageview event to the queue.
	window[gaName]("create", "{{TrackingID}}", "{{Domain}}");
	window[gaName]("send", "pageview");

	// Create a function that wraps `window[gaName]`.
	// This allows dependant modules to use `window[gaName]` without knowingly
	// programming against a global object.
	module = function () {
		window[gaName].apply(this, arguments);
	};

	// Asynchronously load Google Analytics, letting it take over our `window[gaName]`
	// object after it loads. This allows us to add events to `window[gaName]` even
	// before the library has fully loaded.
	require(["//www.google-analytics.com/analytics.js"]);

	return module;

});