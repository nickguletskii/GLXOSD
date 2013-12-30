/**
 * This jQuery plugin resizes a textarea to adapt it automatically to the content.
 * @author Amaury Carrade
 * @version 1.1
 * 
 * @example $('textarea').autoResize({
 *              animate:   {                            // @see .animate()
 *              	enabled:    true,                   // Default: false
 * 					duration:   'fast',                 // Default: 100
 *					complete:   function() {},          // Default: null
 *					step:       function(now, fx) {}    // Default: null
 *              },
 *              maxHeight: '500px'                      // Default: null (unlimited)
 *          });
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lessier General Public License version 3 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lessier General Public License for more details.
 *
 * You should have received a copy of the GNU Lessier General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

(function($) {
	$(document).ready(function() {
		$('body').append('<div id="autoResizeTextareaCopy" style="box-sizing: border-box; -moz-box-sizing: border-box;  -ms-box-sizing: border-box; -webkit-box-sizing: border-box; visibility: hidden;"></div>');
		var $copy = $('#autoResizeTextareaCopy');
		
		function autoSize($textarea, options) { 
			// The copy must have the same padding, the same dimentions and the same police than the original.
			$copy.css({
				fontFamily:     $textarea.css('fontFamily'),
				fontSize:       $textarea.css('fontSize'),
				padding:        $textarea.css('padding'),
				paddingLeft:    $textarea.css('paddingLeft'),
				paddingRight:   $textarea.css('paddingRight'),
				paddingTop:     $textarea.css('paddingTop'), 
				paddingBottom:  $textarea.css('paddingBottom'), 
				width:          $textarea.css('width')
			});
			$textarea.css('overflow', 'hidden');
			
			// Copy textarea contents; browser will calculate correct height of copy.
			var text = $textarea.val().replace(/\n/g, '<br/>');
			$copy.html(text + '<br />');
			
			// Then, we get the height of the copy and we apply it to the textarea.
			var newHeight = $copy.css('height');
			$copy.html(''); // We do this because otherwise, a large void appears in the page if the textarea has a high height.
			if(parseInt(newHeight) != 0) {
				if((options.maxHeight != null && parseInt(newHeight) < parseInt(options.maxHeight)) || options.maxHeight == null) {
					if(options.animate.enabled) {
						$textarea.animate({ 
							height: newHeight 
						}, {
							duration: options.animate.duration,
							complete: options.animate.complete,
							step:     options.animate.step,
							queue:    false
						});
					}
					else {
						$textarea.css('height', newHeight);
					}
					
					$textarea.css('overflow-y', 'hidden');
				}
				else {
					$textarea.css('overflow-y', 'scroll');
				}
			}
		}
		
		$.fn.autoResize = function(options) { 
			var $this = $(this),
			    defaultOptions = {
					animate: {
						enabled:   false,
						duration:  100,
						complete:  null,
						step:      null
					},
					maxHeight:     null
				};
			
			options = (options == undefined) ? {} : options;
			options = $.extend(true, defaultOptions, options);

			$this.change ( function() { autoSize($this, options); } ) 
				 .keydown( function() { autoSize($this, options); } ) 
				 .keyup  ( function() { autoSize($this, options); } )
				 .focus  ( function() { autoSize($this, options); } );

			// No animations on startup
			startupOptions = options;
			startupOptions.animate.enabled = false;
			autoSize($this, startupOptions);
		};
	});
})(jQuery);
