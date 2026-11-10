# SVG Rendering

## 1. Context

The placement engine can generate and optimize label positions, but it currently has no mechanism to visualize the resulting placement solutions.

A rendering component is therefore required to make the engine's output observable and facilitate the inspection of placement results.

The existing architecture provides the necessary domain models:

- `Chart` defines the plot area and label targets.
- `LabelTarget` associates each label with its anchor point.
- `Placement` represents the final position of a label.
- `Rectangle` provides the geometric boundaries required for rendering.

The rendering implementation must preserve the independence of the domain and application layers from presentation technologies.

## 2. Decision

A dedicated SVG rendering component will be introduced in a new rendering layer.

The initial implementation will use a concrete `SvgRenderer` class.

Its public interface will be:

```cpp
std::string render(
    const domain::chart::Chart& chart,
    const std::vector<domain::placement::Placement>& placements
) const;
```

The renderer will receive the original chart and the corresponding placement solution and return a complete SVG document as a string.

The renderer will not modify its input objects.

### 2.1 Rendering Responsibilities

The SVG document will represent:

- The chart plot area.
- Label anchor points.
- Final label positions.
- Connections between anchors and their corresponding labels.
- Visual indicators for label collisions.
- Visual indicators for labels extending beyond the plot area.

The renderer is responsible for converting the existing domain objects into their SVG representation.

### 2.2 Input Validation

The renderer will validate that:

1. The number of placements matches the number of label targets.
2. Each placement contains the same label as the corresponding target at the same index.
3. Label text contains valid UTF-8 sequences and only characters permitted by XML 1.0.

Invalid placement solutions or label text will result in `std::invalid_argument`.

UTF-8 validation rejects malformed, truncated, overlong, and otherwise invalid sequences. XML character validation rejects characters that cannot legally appear in an XML 1.0 document.

Valid Unicode characters and permitted XML whitespace characters, including tab, line feed, and carriage return, are preserved.

This validation ensures that every rendered placement can be associated with its original label target and that label text can be safely included in the generated SVG document.


### 2.3 Coordinate Transformation

The domain uses a Cartesian coordinate system where:

- The X-axis increases to the right.
- The Y-axis increases upwards.

SVG uses a coordinate system where:

- The X-axis increases to the right.
- The Y-axis increases downwards.

The renderer will handle this difference without modifying the domain geometry.

The initial transformation will be:

- `svg_x = domain_x`
- `svg_y = -domain_y`

Rectangle dimensions will remain unchanged.

### 2.4 Viewport

The SVG viewBox will include:

- The complete plot area.
- All label placements.
- All label anchor points.

A fixed visual margin of 10 coordinate units will be added around the calculated boundaries.

This ensures that placements extending beyond the plot area remain visible.

### 2.5 SVG Output

The renderer will generate SVG markup directly as text without external graphics dependencies.

The output will use:

- SVG elements for geometric shapes and text.
- XML escaping for label text, including the five predefined XML special characters.
- UTF-8 validation and XML 1.0 character validation before serialization.
- Locale-independent numeric formatting.
- Deterministic output for identical inputs.

Valid Unicode characters are preserved in the generated SVG. Invalid UTF-8 sequences and characters prohibited by XML 1.0 are rejected.

The initial implementation will not provide direct filesystem operations.

### 2.6 Collision and Boundary Visualization

Collision detection remains the responsibility of `CollisionDetector`.

The renderer may use collision detection results to visually distinguish colliding labels, but it will not implement collision detection independently.

Boundary visualization will use the existing geometric information to identify placements extending beyond the plot area.

The rendering layer will not duplicate placement scoring or optimization logic.

The renderer will instantiate and use the existing `CollisionDetector` internally. This introduces a dependency from the rendering layer to the application layer, while keeping the public rendering interface independent of collision detection details.

## 3. Rationale

SVG was selected because it:

- Is a text-based vector graphics format.
- Can be inspected and displayed directly in modern browsers.
- Does not require external graphics libraries for the initial implementation.
- Provides a convenient format for automated testing.
- Allows geometric elements and text to be represented independently.

Returning the SVG document as a string keeps the renderer independent of filesystem operations and allows consumers to decide how the generated output should be used.

A dedicated rendering layer prevents presentation concerns from leaking into the domain and application layers.

## 4. Consequences

### Positive

- Placement solutions become visually inspectable.
- The domain remains independent of SVG.
- Rendering can be tested without filesystem operations.
- The generated output is deterministic.
- The rendering implementation can evolve independently of placement algorithms.
- Future rendering formats can be introduced without modifying existing domain models.

### Limitations

- The initial implementation supports SVG only.
- The renderer provides a technical visualization rather than a complete charting solution.
- The coordinate transformation and XML serialization must be handled explicitly.
- SVG output may require additional formatting and styling capabilities in future versions.

## 5. Alternatives Considered

### Direct SVG Generation in the Domain Layer

Generating SVG directly from domain objects would introduce presentation concerns into the domain model.

This approach was rejected because it would couple core business concepts to a specific output format.

### External Graphics Libraries

External graphics libraries could provide additional rendering capabilities.

They were not selected because the initial requirements can be satisfied through direct SVG generation without introducing additional dependencies.

### Generic Renderer Abstraction

A generic renderer interface could support multiple output formats.

It was not introduced because SVG is currently the only planned rendering implementation. Such an abstraction would add complexity without providing an immediate benefit.

### Direct Filesystem Output

The renderer could write SVG documents directly to files.

This approach was rejected because filesystem operations are not part of the renderer's core responsibility. Returning a string provides greater flexibility and simplifies testing.

## 6. Status

Accepted.
